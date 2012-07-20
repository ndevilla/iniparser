
/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.c
   @author  N. Devillard
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/
/*---------------------------- Includes ------------------------------------*/
#include <ctype.h>
#include "iniparser.h"

/*---------------------------- Defines -------------------------------------*/
#define ASCIILINESZ         (1024)
#define INI_INVALID_KEY     ((char*)-1)

/*---------------------------------------------------------------------------
                        Private to this module
 ---------------------------------------------------------------------------*/
/**
 * This enum stores the status for each parsed line (internal use only).
 */
typedef enum _line_status_ {
    LINE_UNPROCESSED,
    LINE_ERROR,
    LINE_EMPTY,
    LINE_COMMENT,
    LINE_SECTION,
    LINE_VALUE
} line_status ;

/*-------------------------------------------------------------------------*/
/**
  @brief    Convert a string to lowercase.
  @param    s   String to convert.
  @return   ptr to statically allocated string.

  This function returns a pointer to a statically allocated string
  containing a lowercased version of the input string. Do not free
  or modify the returned string! Since the returned string is statically
  allocated, it will be modified at each function call (not re-entrant).
 */
/*--------------------------------------------------------------------------*/
static const char * strlwc(const char * s)
{
    static char l[ASCIILINESZ+1];
    int i = 0;

    if(s==NULL)
    {
        return NULL ;
    }
    memset(l, 0, ASCIILINESZ+1);

    while (i<ASCIILINESZ && s[i])
    {
        l[i] = (char)tolower((int)s[i]);
        ++i;
    }
    l[ASCIILINESZ]=(char)0;
    return l;
}

/* Private: add an entry to the dictionary */
static void iniparser_add_entry(dictionary * d,
                                const char * const sec,
                                const char * const key,
                                const char * const val)
{
    char longkey[(2*ASCIILINESZ)+1];

    /* Make a key as section:keyword */
    if(NULL != key)
    {
        sprintf(longkey, "%s:%s", sec, key);
    }
    else
    {
        strcpy(longkey, sec);
    }

    /* Add (key,val) to dictionary */
    dictionary_set(d, longkey, val);
    return;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Remove blanks at the beginning and the end of a string.
  @param    s   String to parse.
  @return   ptr to statically allocated string.

  This function returns a pointer to a statically allocated string,
  which is identical to the input string, except that all blank
  characters at the end and the beg. of the string have been removed.
  Do not free or modify the returned string! Since the returned string
  is statically allocated, it will be modified at each function call
  (not re-entrant).
 */
/*--------------------------------------------------------------------------*/
static const char * strstrip(const char * s)
{
    static char l[ASCIILINESZ+1];
    char * last ;
    
    if (s==NULL)
    {
        return NULL ;
    }
    
    while (isspace((int)*s) && *s) s++;
    
    memset(l, 0, ASCIILINESZ+1);
    strcpy(l, s);
    last = l + strlen(l);
    while(last > l)
    {
        if(!isspace((int)*(last-1)))
        {
            break;
        }
        --last;
    }
    *last = (char)0;
    return l ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get number of sections in a dictionary
  @param    d   Dictionary to examine
  @return   int Number of sections found in dictionary

  This function returns the number of sections found in a dictionary.
  The test to recognize sections is done on the string stored in the
  dictionary: a section name is given as "section" whereas a key is
  stored as "section:key", thus the test looks for entries that do not
  contain a colon.

  This clearly fails in the case a section name contains a colon, but
  this should simply be avoided.

  This function returns -1 in case of error.
 */
/*--------------------------------------------------------------------------*/

ssize_t iniparser_getnsec(const dictionary * const d)
{
    size_t i = 0;
    ssize_t nsec = 0;

    if (d==NULL)
    {
        return -1;
    }

    for ( ; i < d->size; ++i)
    {
        if(NULL == d->key[i])
        {
            continue;
        }

        if(NULL == strchr(d->key[i], ':'))
        {
            ++nsec;
        }
    }
    return nsec;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get name for section n in a dictionary.
  @param    d   Dictionary to examine
  @param    n   Section number (from 0 to nsec-1).
  @return   Pointer to char string

  This function locates the n-th section in a dictionary and returns
  its name as a pointer to a string statically allocated inside the
  dictionary. Do not free or modify the returned string!

  This function returns NULL in case of error.
 */
/*--------------------------------------------------------------------------*/
const char * iniparser_getsecname(const dictionary * const d, size_t n)
{
    size_t i = 0;
    size_t foundsec = 0;

    if((NULL == d) || (n < 0))
    {
        return NULL;
    }

    for( ; i < d->size; ++i)
    {
        if(NULL == d->key[i])
        {
            continue ;
        }
        if(NULL == strchr(d->key[i], ':'))
        {
            ++foundsec;
            if(foundsec>n)
            {
                break;
            }
        }
    }
    if(foundsec <= n)
    {
        return NULL;
    }
    return d->key[i];
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump.
  @param    f   Opened file pointer to dump to.
  @return   void

  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify @c stderr
  or @c stdout as output files. This function is meant for debugging
  purposes mostly.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump(const dictionary * const d, FILE * const f)
{
    size_t i = 0;

    if((NULL == d) || (NULL == f))
    {
        return;
    }
    for( ; i < d->size; ++i)
    {
        if(NULL == d->key[i])
        {
            continue;
        }
        if (NULL != d->val[i])
        {
            fprintf(f, "[%s]=[%s]\n", d->key[i], d->val[i]);
        }
        else
        {
            fprintf(f, "[%s]=UNDEF\n", d->key[i]);
        }
    }
    return;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary to a loadable ini file
  @param    d   Dictionary to dump
  @param    f   Opened file pointer to dump to
  @return   void

  This function dumps a given dictionary into a loadable ini file.
  It is Ok to specify @c stderr or @c stdout as output files.
 */
/*--------------------------------------------------------------------------*/

void iniparser_dump_ini(const dictionary * const d, FILE * const f)
{
    size_t        i       = 0;
    ssize_t       nsec    = 0;
    const char *  secname = NULL;

    if((NULL == d) || (NULL == f))
    {
        return;
    }

    nsec = iniparser_getnsec(d);
    if(nsec<1)
    {
        /* No section in file: dump all keys as they are */
        for( ; i<d->size; ++i)
        {
            if(NULL == d->key[i])
            {
                continue;
            }
            fprintf(f, "%s = %s\n", d->key[i], d->val[i]);
        }
        return;
    }
    for ( ; i<nsec; ++i)
    {
        secname = iniparser_getsecname(d, i) ;
        iniparser_dumpsection_ini(d, secname, f) ;
    }
    fprintf(f, "\n");
    return;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary section to a loadable ini file
  @param    d   Dictionary to dump
  @param    s   Section name of dictionary to dump
  @param    f   Opened file pointer to dump to
  @return   void

  This function dumps a given section of a given dictionary into a loadable ini
  file.  It is Ok to specify @c stderr or @c stdout as output files.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dumpsection_ini(const dictionary * const d, const char * s, FILE * const f)
{
    size_t  j=0;
    char    keym[ASCIILINESZ+1];
    size_t  seclen;

    if((NULL == d) || (NULL == f))
    {
        return;
    }

    if (! iniparser_find_entry(d, s))
    {
        return;
    }

    seclen  = strlen(s);
    fprintf(f, "\n[%s]\n", s);
    sprintf(keym, "%s:", s);

    for( ; j<d->size; ++j)
    {
        if (NULL == d->key[j])
        {
            continue;
        }

        if (!strncmp(d->key[j], keym, seclen+1))
        {
            fprintf(f,
                    "%-30s = %s\n",
                    d->key[j]+seclen+1,
                    d->val[j] ? d->val[j] : "");
        }
    }
    fprintf(f, "\n");
    return;
}




/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, return NULL if not found
  @param    d   Dictionary to search
  @param    key Key string to look for
  @return   pointer to statically allocated character string, or NULL.

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  NULL is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.

  This function is only provided for backwards compatibility with 
  previous versions of iniparser. It is recommended to use
  iniparser_getstring() instead.
 */
/*--------------------------------------------------------------------------*/
const char * iniparser_getstr(const dictionary * const d, const char * key)
{
    return iniparser_getstring(d, key, NULL);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the number of keys in a section of a dictionary.
  @param    d   Dictionary to examine
  @param    s   Section name of dictionary to examine
  @return   Number of keys in section
 */
/*--------------------------------------------------------------------------*/
int iniparser_getsecnkeys(const dictionary * const d, const char * s)
{
    size_t  seclen;
    int     nkeys = 0;
    char    keym[ASCIILINESZ+1];
    int j = 0;

    if(d==NULL)
    {
        return nkeys;
    }

    if(! iniparser_find_entry(d, s))
    {
        return nkeys;
    }

    seclen  = strlen(s);
    sprintf(keym, "%s:", s);

    for ( ; j<d->size ; ++j)
    {
        if(NULL == d->key[j])
        {
            continue;
        }
        if(!strncmp(d->key[j], keym, seclen+1))
        {
            ++nkeys;
        }
    }
    return nkeys;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the number of keys in a section of a dictionary.
  @param    d   Dictionary to examine
  @param    s   Section name of dictionary to examine
  @return   pointer to statically allocated character strings

  This function queries a dictionary and finds all keys in a given section.
  Each pointer in the returned char pointer-to-pointer is pointing to
  a string allocated in the dictionary; do not free or modify them.
  
  This function returns NULL in case of error.
 */
/*--------------------------------------------------------------------------*/
const char ** iniparser_getseckeys(const dictionary * const d, const char * s)
{
    const char **keys = NULL;

    size_t  i = 0;
    size_t  j = 0;
    char    keym[ASCIILINESZ+1];
    size_t  seclen = 0;
    int     nkeys  = 0;

    if(NULL == d)
    {
        return keys;
    }

    if(! iniparser_find_entry(d, s))
    {
        return keys;
    }

    nkeys = iniparser_getsecnkeys(d, s);

    keys = (const char**) malloc(nkeys*sizeof(const char*));

    seclen  = strlen(s);
    sprintf(keym, "%s:", s);

    for( ; j<d->size ; ++j)
    {
        if(NULL == d->key[j])
        {
            continue;
        }

        if( ! strncmp(d->key[j], keym, seclen+1))
        {
            keys[i] = d->key[j];
            ++i;
        }
    }
    return keys;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @param    def     Default value to return if key not found.
  @return   pointer to statically allocated character string

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the pointer passed as 'def' is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.
 */
/*--------------------------------------------------------------------------*/
const char * iniparser_getstring(const dictionary * const d, const char * key, const char * def)
{
    if (d==NULL || key==NULL)
    {
        return def;
    }
    else
    {
        return dictionary_get(d, strlwc(key), def);
    }

}



/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  Supported values for integers include the usual C notation
  so decimal, octal (starting with 0) and hexadecimal (starting with 0x)
  are supported. Examples:

  "42"      ->  42
  "042"     ->  34 (octal -> decimal)
  "0x42"    ->  66 (hexa  -> decimal)

  Warning: the conversion may overflow in various ways. Conversion is
  totally outsourced to strtol(), see the associated man page for overflow
  handling.

  Credits: Thanks to A. Becker for suggesting strtol()
 */
/*--------------------------------------------------------------------------*/
int iniparser_getint(dictionary * d, const char * key, int notfound)
{
    const char * const str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if(INI_INVALID_KEY == str)
    {
        return notfound;
    }
    {
        return (int)strtol(str, NULL, 0);
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   double

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.
 */
/*--------------------------------------------------------------------------*/
double iniparser_getdouble(dictionary * d, const char * key, double notfound)
{
    const char * const str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (str==INI_INVALID_KEY)
    {
        return notfound;
    }
    else
    {
        return atof(str);
    }
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  A true boolean is found if one of the following is matched:

  - A string starting with 'y'
  - A string starting with 'Y'
  - A string starting with 't'
  - A string starting with 'T'
  - A string starting with '1'

  A false boolean is found if one of the following is matched:

  - A string starting with 'n'
  - A string starting with 'N'
  - A string starting with 'f'
  - A string starting with 'F'
  - A string starting with '0'

  The notfound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 */
/*--------------------------------------------------------------------------*/
int iniparser_getboolean(dictionary * d, const char * key, int notfound)
{
    const char * const c = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (INI_INVALID_KEY == c)
    {
        return notfound;
    }

    switch(c[0])
    {
        case 'y':
        case 'Y':
        case '1':
        case 't':
        case 'T':
            return 1;
        case 'n':
        case 'N':
        case '0':
        case 'f':
        case 'F':
            return 0;
        default:
            return notfound;
    }
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Finds out if a given entry exists in a dictionary
  @param    ini     Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise

  Finds out if a given entry exists in the dictionary. Since sections
  are stored as keys with NULL associated values, this is the only way
  of querying for the presence of sections in a dictionary.
 */
/*--------------------------------------------------------------------------*/
int iniparser_find_entry(const dictionary  * const  ini,
                         const char  *  const entry
)
{
    return (iniparser_getstring(ini, entry, INI_INVALID_KEY)!=INI_INVALID_KEY);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.

  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
  It is Ok to set val to NULL.
 */
/*--------------------------------------------------------------------------*/
int iniparser_set(dictionary * ini, const char * entry, const char * val)
{
    return dictionary_set(ini, strlwc(entry), val) ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.

  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
  It is Ok to set val to NULL.
 */
/*--------------------------------------------------------------------------*/

int iniparser_setstr(dictionary * ini, char * entry, char * val)
{
    return dictionary_set(ini, strlwc(entry), val);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   Entry to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/
void iniparser_unset(dictionary * ini, const char * entry)
{
    dictionary_unset(ini, strlwc(entry));
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Load a single line from an INI file
  @param    input_line  Input line, may be concatenated multi-line input
  @param    section     Output space to store section
  @param    key         Output space to store key
  @param    value       Output space to store value
  @return   line_status value
 */
/*--------------------------------------------------------------------------*/
static line_status iniparser_line(const char * input_line,
                                  char * section,
                                  char * key,
                                  char * value)
{   
    line_status sta;
    char        line[ASCIILINESZ+1];
    size_t      len;

    strcpy(line, strstrip(input_line));
    len = strlen(line);

    sta = LINE_UNPROCESSED ;
    if (len<1)
    {
        /* Empty line */
        sta = LINE_EMPTY ;
    }
    else if (line[0]=='#' || line[0]==';')
    {
        /* Comment line */
        sta = LINE_COMMENT ; 
    }
    else if (line[0]=='[' && line[len-1]==']')
    {
        /* Section name */
        sscanf(line, "[%[^]]", section);
        strcpy(section, strstrip(section));
        strcpy(section, strlwc(section));
        sta = LINE_SECTION ;
    }
    else if(sscanf (line, "%[^=] = \"%[^\"]\"", key, value) == 2
         || sscanf (line, "%[^=] = '%[^\']'",   key, value) == 2
         || sscanf (line, "%[^=] = %[^;#]",     key, value) == 2)
    {
        /* Usual key=value, with or without comments */
        strcpy(key, strstrip(key));
        strcpy(key, strlwc(key));
        strcpy(value, strstrip(value));
        /*
         * sscanf cannot handle '' or "" as empty values
         * this is done here
         */
        if (!strcmp(value, "\"\"") || (!strcmp(value, "''")))
        {
            value[0]=0 ;
        }
        sta = LINE_VALUE ;
    }
    else if(sscanf(line, "%[^=] = %[;#]", key, value) ==2
         || sscanf(line, "%[^=] %[=]", key, value) == 2)
    {
        /*
         * Special cases:
         * key=
         * key=;
         * key=#
         */
        strcpy(key, strstrip(key));
        strcpy(key, strlwc(key));
        value[0] = 0;
        sta = LINE_VALUE;
    }
    else
    {
        /* Generate syntax error */
        sta = LINE_ERROR;
    }
    return sta;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Parse an ini file and return an allocated dictionary object
  @param    ininame Name of the ini file to read.
  @return   Pointer to newly allocated dictionary

  This is the parser for ini files. This function is called, providing
  the name of the file to be read. It returns a dictionary object that
  should not be accessed directly, but through accessor functions
  instead.

  The returned dictionary must be freed using iniparser_freedict().
 */
/*--------------------------------------------------------------------------*/
dictionary * iniparser_load(const char * ininame)
{
    FILE * in ;

    char line    [ASCIILINESZ+1];
    char section [ASCIILINESZ+1];
    char key     [ASCIILINESZ+1];
    char tmp     [ASCIILINESZ+1];
    char val     [ASCIILINESZ+1];

    int  last = 0;
    int  len;
    int  lineno = 0;
    int  errs = 0;


    dictionary * dict;
    in=fopen(ininame, "r");
    if(in==NULL)
    {
        fprintf(stderr, "iniparser: cannot open %s\n", ininame);
        return NULL ;
    }

    dict = dictionary_new(0);
    if (!dict)
    {
        fclose(in);
        return NULL ;
    }

    memset(line,    0, ASCIILINESZ);
    memset(section, 0, ASCIILINESZ);
    memset(key,     0, ASCIILINESZ);
    memset(val,     0, ASCIILINESZ);
    last=0 ;

    while (fgets(line+last, ASCIILINESZ-last, in)!=NULL)
    {
        ++lineno;
        len = (int)strlen(line)-1;
        if (len==0)
        {
            continue;
        }
        /* Safety check against buffer overflows */
        if (line[len]!='\n' && !feof(in))
        {
            fprintf(stderr,
                    "iniparser: input line too long in %s (%d)\n",
                    ininame,
                    lineno);
            dictionary_del(dict);
            fclose(in);
            return NULL;
        }
        /* Get rid of \n and spaces at end of line */
        while ((len>=0) &&
                ((line[len]=='\n') || (isspace(line[len]))))
        {
            line[len] = 0;
            --len;
        }
        /* Detect multi-line */
        if (line[len]=='\\')
        {
            /* Multi-line value */
            last = len;
            continue;
        }
        else
        {
            last = 0;
        }
        switch (iniparser_line(line, section, key, val))
        {
            case LINE_EMPTY:
            case LINE_COMMENT:
                break;
            case LINE_SECTION:
                errs = dictionary_set(dict, section, NULL);
                break ;
            case LINE_VALUE:
                sprintf(tmp, "%s:%s", section, key);
                errs = dictionary_set(dict, tmp, val) ;
                break ;
            case LINE_ERROR:
                fprintf(stderr, "iniparser: syntax error in %s (%d):\n",
                        ininame,
                        lineno);
                fprintf(stderr, "-> %s\n", line);
                ++errs;
                break;
            default:
                break;
        }
        memset(line, 0, ASCIILINESZ);
        last = 0;
        if(errs < 0)
        {
            fprintf(stderr, "iniparser: memory allocation failure\n");
            break;
        }
    }
    if(errs)
    {
        dictionary_del(dict);
        dict = NULL ;
    }
    fclose(in);
    return dict ;
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Free all memory associated to an ini dictionary
  @param    d Dictionary to free
  @return   void

  Free all memory associated to an ini dictionary.
  It is mandatory to call this function before the dictionary object
  gets out of the current context.
 */
/*--------------------------------------------------------------------------*/

void iniparser_freedict(dictionary * d)
{
    dictionary_del(d);
}

