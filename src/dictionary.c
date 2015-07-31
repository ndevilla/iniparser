/*-------------------------------------------------------------------------*/
/**
   @file    dictionary.c
   @author  N. Devillard
   @brief   Implements a dictionary for string variables.

   This module implements a simple dictionary object, i.e. a list
   of string/string associations. This object is useful to store e.g.
   informations retrieved from a configuration file (ini files).
*/
/*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                                Includes
 ---------------------------------------------------------------------------*/
#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/** Maximum value size for integers and doubles. */
#define MAXVALSZ    1024

/** Minimal allocated number of entries in a dictionary */
#define DICTMINSZ   128

/** Invalid key token */
#define DICT_INVALID_KEY    ((char*)-1)

/*---------------------------------------------------------------------------
                            Private functions
 ---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
  @brief    Duplicate a string
  @param    s String to duplicate
  @return   Pointer to a newly allocated string, to be freed with free()

  This is a replacement for strdup(). This implementation is provided
  for systems that do not have it.
 */
/*--------------------------------------------------------------------------*/
static char * xstrdup(const char * const s)
{
    char * t;
    size_t len;
    if(!s)
    {
        return NULL;
    }
    len = strlen(s) + 1;
    t = (char*)malloc(len);
    if(NULL != t)
    {
        memcpy(t, s, len);
    }
    return t;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Double the size of the dictionary
  @param    d Dictionary to grow
  @return   This function returns non-zero in case of failure
 */
/*--------------------------------------------------------------------------*/
static int dictionary_grow(dictionary * const d)
{
    char        ** new_val;
    char        ** new_key;
    unsigned     * new_hash;

    new_val  = (char**)    calloc(d->size * 2, sizeof(*d->val));
    new_key  = (char**)    calloc(d->size * 2, sizeof(*d->key));
    new_hash = (unsigned*) calloc(d->size * 2, sizeof(*d->hash));

    if(   (NULL == new_val)
       || (NULL == new_key)
       || (NULL == new_hash))
    {
        /* An allocation failed, leave the dictionary unchanged */
        free(new_val);
        free(new_key);
        free(new_hash);
        return -1 ;
    }

    /* Initialize the newly allocated space */
    memcpy(new_val,  d->val,  d->size * sizeof(char *));
    memcpy(new_key,  d->key,  d->size * sizeof(char *));
    memcpy(new_hash, d->hash, d->size * sizeof(unsigned));

    /* Delete previous data */
    free(d->val);
    free(d->key);
    free(d->hash);

    /* Actually update the dictionary */
    d->size *= 2 ;

    d->val = new_val;
    d->key = new_key;
    d->hash = new_hash;

    return 0 ;
}

/*---------------------------------------------------------------------------
                              Function codes
 ---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
  @brief    Compute the hash key for a string.
  @param    key     Character string to use for key.
  @return   1 unsigned int on at least 32 bits.

  This hash function has been taken from an Article in Dr Dobbs Journal.
  This is normally a collision-free function, distributing keys evenly.
  The key is stored anyway in the struct so that collision can be avoided
  by comparing the key itself in last resort.
 */
/*--------------------------------------------------------------------------*/
unsigned dictionary_hash(const char * const key)
{
    size_t      len;
    unsigned    hash;
    size_t      i;

    if(NULL == key)
    {
        return 0;
    }

    len = strlen(key);
    for(hash=0, i=0; i < len; ++i)
    {
        hash += (unsigned int)key[i];
        hash += (hash<<10);
        hash ^= (hash>>6);
    }
    hash += (hash <<3);
    hash ^= (hash >>11);
    hash += (hash <<15);
    return hash;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Create a new dictionary object.
  @param    size    Optional initial size of the dictionary.
  @return   1 newly allocated dictionary objet.

  This function allocates a new dictionary object of given size and returns
  it. If you do not know in advance (roughly) the number of entries in the
  dictionary, give size=0.
 */
/*-------------------------------------------------------------------------*/
dictionary * dictionary_new(size_t size)
{
    dictionary * d = NULL;

    /* If no size was specified, allocate space for DICTMINSZ */
    if(size<DICTMINSZ)
    {
        size = DICTMINSZ;
    }

    d = (dictionary*) calloc(1, sizeof(*d)) ;

    if(NULL != d)
    {
        d->size = size;
        d->val  = (char**)    calloc(size, sizeof(*(d->val)));
        d->key  = (char**)    calloc(size, sizeof(*(d->key)));
        d->hash = (unsigned*) calloc(size, sizeof(*(d->hash)));
    }
    return d ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete a dictionary object
  @param    d   dictionary object to deallocate.
  @return   void

  Deallocate a dictionary object and all memory associated to it.
 */
/*--------------------------------------------------------------------------*/
void dictionary_del(dictionary * const d)
{
    ssize_t  i = 0;
    if(NULL == d)
    {
        return;
    }

    for(; i < d->size; ++i)
    {
        free(d->key[i]);
        free(d->val[i]);
    }
    free(d->val);
    free(d->key);
    free(d->hash);
    free(d);
}



/*-------------------------------------------------------------------------*/
/**
  @brief    Get a value from a dictionary.
  @param    d       Dictionary object to search.
  @param    key     Key to look for in the dictionary.
  @param    def     Default value to return if key not found.
  @return   1 pointer to internally allocated character string.

  This function locates a key in a dictionary and returns a pointer to its
  value, or the passed 'def' pointer if no such key can be found in
  dictionary. The returned character pointer points to data internal to the
  dictionary object, you should not try to free it or modify it.
 */
/*--------------------------------------------------------------------------*/
const char * dictionary_get(const dictionary * const d,
                            const char       * const key,
                            const char       * const def)
{
    unsigned    hash;
    ssize_t     i;

    hash = dictionary_hash(key);
    for (i=0; i < d->size; ++i)
    {
        if(NULL == d->key[i])
        {
            continue;
        }
        /* Compare hash */
        if(hash == d->hash[i])
        {
            /* Compare string, to avoid hash collisions */
            if( ! strcmp(key, d->key[i]))
            {
                return d->val[i];
            }
        }
    }
    return def;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get a value from a dictionary, as a char.
  @param    d        ictionary object to search.
  @param    key      Key to look for in the dictionary.
  @param    def      Default value for the key if not found.
  @return   char    

  This function locates a key in a dictionary using dictionary_get,
  and returns the first char of the found string.
 */
/*--------------------------------------------------------------------------*/
char dictionary_getchar(const dictionary * const d,
                        const char       * const key,
                        const char               def)
{
    const char * const v = dictionary_get(d,key,DICT_INVALID_KEY);
    if(DICT_INVALID_KEY != v)
    {
        return v[0];
    }
    return def;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Get a value from a dictionary, as an int.
  @param    d        Dictionary object to search.
  @param    key      Key to look for in the dictionary.
  @param    def      Default value for the key if not found.
  @return   int

  This function locates a key in a dictionary using dictionary_get,
  and applies atoi on it to return an int. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
int dictionary_getint(const dictionary * const d,
                      const char       * const key,
                      const int                def)
{
    const char * const v = dictionary_get(d, key, DICT_INVALID_KEY);
    if(DICT_INVALID_KEY != v)
    {
        return atoi(v);
    }
    return def;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Get a value from a dictionary, as a double.
  @param    d        Dictionary object to search.
  @param    key      Key to look for in the dictionary.
  @param    def      Default value for the key if not found.
  @return   double

  This function locates a key in a dictionary using dictionary_get,
  and applies atof on it to return a double. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
double dictionary_getdouble(const dictionary * const d,
                            const char       * const key,
                            const double             def)
{
    const char * const v=dictionary_get(d,key,DICT_INVALID_KEY);
    if(DICT_INVALID_KEY != v)
    {
        return atof(v);
    }
    return def;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Set a value in a dictionary.
  @param    d       dictionary object to modify.
  @param    key     Key to modify or add.
  @param    val     Value to add.
  @return   int     0 if Ok, anything else otherwise

  If the given key is found in the dictionary, the associated value is
  replaced by the provided one. If the key cannot be found in the
  dictionary, it is added to it.

  It is Ok to provide a NULL value for val, but NULL values for the dictionary
  or the key are considered as errors: the function will return immediately
  in such a case.

  Notice that if you dictionary_set a variable to NULL, a call to
  dictionary_get will return a NULL value: the variable will be found, and
  its value (NULL) is returned. In other words, setting the variable
  content to NULL is equivalent to deleting the variable from the
  dictionary. It is not possible (in this implementation) to have a key in
  the dictionary without value.

  This function returns non-zero in case of failure.
 */
/*--------------------------------------------------------------------------*/
int dictionary_set(      dictionary * const d,
                   const char       * const key,
                   const char       * const val)
{
    ssize_t      i;
    unsigned    hash;

    if(   (NULL == d)
       || (NULL == key))
    {
        return -1;
    }

    /* Compute hash for this key */
    hash = dictionary_hash(key);

    /* Find if value is already in dictionary */
    if(d->n>0)
    {
        for(i=0; i < d->size; ++i)
        {
            if(d->key[i]==NULL)
            {
                continue;
            }
            if(hash==d->hash[i])
            {   /* Same hash value */
                if(!strcmp(key, d->key[i]))
                {   /* Same key */
                    /* Found a value: modify and return */
                    free(d->val[i]);
                    d->val[i] = (val ? xstrdup(val) : NULL);

                    /* Value has been modified: return */
                    return 0;
                }
            }
        }
    }
    /* Add a new value */
    /* See if dictionary needs to grow */
    if(d->n == d->size)
    {
        if(0 != dictionary_grow(d))
        {
            return -1;
        }
    }

    /*
     * Insert key in the first empty slot. Start at d->n and wrap at d->size.
     * Because d->n < d->size this will necessarily terminate.
     */
    for (i=d->n ; d->key[i] ; )
    {
        if(++i == d->size)
        {
            i = 0;
        }
    }
    /* Copy key */
    d->key[i]  = xstrdup(key);
    d->val[i]  = (val ? xstrdup(val) : NULL);
    d->hash[i] = hash;
    ++(d->n);
    return 0;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete a key in a dictionary
  @param    d       Dictionary object to modify.
  @param    key     Key to remove.
  @return   void

  This function deletes a key in a dictionary. Nothing is done if the
  key cannot be found.
 */
/*--------------------------------------------------------------------------*/
void dictionary_unset(      dictionary * const d,
                      const char       * const key)
{
    unsigned    hash;
    ssize_t      i;

    if(   (NULL == key)
       || (NULL == d))
    {
        return;
    }

    hash = dictionary_hash(key);
    for(i=0; i < d->size; ++i)
    {
        if(d->key[i]==NULL)
        {
            continue;
        }

        /* Compare hash */
        if(hash == d->hash[i])
        {
            /* Compare string, to avoid hash collisions */
            if( ! strcmp(key, d->key[i]))
            {
                /* Found key */
                break;
            }
        }
    }

    if(i >= d->size)
    {
        /* Key not found */
        return;
    }

    free(d->key[i]);
    d->key[i] = NULL;

    free(d->val[i]);
    d->val[i] = NULL;

    d->hash[i] = 0;
    --(d->n);

    return;
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Set a key in a dictionary, providing an int.
  @param    d        Dictionary to update.
  @param    key      Key to modify or add
  @param    val      Integer value to store (will be stored as a string).
  @return   void

  This helper function calls dictionary_set() with the provided integer
  converted to a string using %d.
 */
/*--------------------------------------------------------------------------*/
void dictionary_setint(      dictionary * const d,
                       const char       * const key,
                       const int                val)
{
    char    sval[MAXVALSZ];
    sprintf(sval, "%d", val);
    dictionary_set(d, key, sval);
}


/*-------------------------------------------------------------------------*/
/**
  @brief    Set a key in a dictionary, providing a double.
  @param    d        Dictionary to update.
  @param    key      Key to modify or add
  @param    val      Double value to store (will be stored as a string).
  @return   void

  This helper function calls dictionary_set() with the provided double
  converted to a string using %g.
 */
/*--------------------------------------------------------------------------*/
void dictionary_setdouble(      dictionary * const d,
                          const char       * const key,
                          const double             val)
{
    char    sval[MAXVALSZ];
    sprintf(sval, "%g", val);
    dictionary_set(d, key, sval);
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump
  @param    f   Opened file pointer.
  @return   void

  Dumps a dictionary onto an opened file pointer. Key pairs are printed out
  as @c [Key]=[Value], one per line. It is Ok to provide stdout or stderr as
  output file pointers.
 */
/*--------------------------------------------------------------------------*/
void dictionary_dump(const dictionary * const d,
                           FILE       * const out)
{
    ssize_t  i ;

    if(   (NULL == d)
       || (NULL == out))
    {
        return;
    }

    if(1 > d->n)
    {
        fprintf(out, "empty dictionary\n");
        return;
    }

    for(i=0; i < d->size; ++i)
    {
        if(d->key[i])
        {
            fprintf(out,
                    "%20s\t[%s]\n",
                    d->key[i],
                      d->val[i]
                    ? d->val[i]
                    : "UNDEF");
        }
    }
}
