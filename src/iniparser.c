/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.c
   @author  N. Devillard
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/
/*---------------------------- Includes ------------------------------------*/
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
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
} line_status;

/*-------------------------------------------------------------------------*/
/**
  @brief    Convert a string to lowercase.
  @param    in   String to convert.
  @param    out Output buffer.
  @param    len Size of the out buffer.
  @return   ptr to the out buffer or NULL if an error occured.

  This function convert a string into lowercase.
  At most len - 1 elements of the input string will be converted.
 */
/*--------------------------------------------------------------------------*/
static const char *strlwc(const char *in, char *out, unsigned len)
{
    unsigned i;

    if (in == NULL || out == NULL || len == 0) return NULL;

    i = 0;

    while (in[i] != '\0' && i < len - 1) {
        out[i] = (char)tolower((int)in[i]);
        i++;
    }

    out[i] = '\0';
    return out;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Remove blanks at the beginning and the end of a string.
  @param    str  String to parse and alter.
  @return   unsigned New size of the string.
 */
/*--------------------------------------------------------------------------*/
static unsigned strstrip(char *s)
{
    char *last = NULL;
    char *dest = s;

    if (s == NULL) return 0;

    last = s + strlen(s);

    while (isspace((unsigned char)*s) && *s) s++;

    while (last > s) {
        if (!isspace((unsigned char) * (last - 1)))
            break;

        last--;
    }

    *last = (char)0;

    memmove(dest, s, last - s + 1);
    return last - s;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Default error callback for iniparser: wraps `fprintf(stderr, ...)`.
 */
/*--------------------------------------------------------------------------*/
static int default_error_callback(const char *format, ...)
{
    int ret;
    va_list argptr;
    va_start(argptr, format);
    ret = vfprintf(stderr, format, argptr);
    va_end(argptr);
    return ret;
}

static int (*iniparser_error_callback)(const char *,
                                       ...) = default_error_callback;

void iniparser_set_error_callback(int (*errback)(const char *, ...))
{
    if (errback) {
        iniparser_error_callback = errback;
    } else {
        iniparser_error_callback = default_error_callback;
    }
}

int iniparser_getnsec(const dictionary *d)
{
    size_t i;
    int nsec;

    if (d == NULL) return -1;

    nsec = 0;

    for (i = 0; i < d->size; i++) {
        if (d->key[i] == NULL)
            continue;

        if (strchr(d->key[i], ':') == NULL) {
            nsec++;
        }
    }

    return nsec;
}

const char *iniparser_getsecname(const dictionary *d, int n)
{
    size_t i;
    int foundsec;

    if (d == NULL || n < 0) return NULL;

    foundsec = 0;

    for (i = 0; i < d->size; i++) {
        if (d->key[i] == NULL)
            continue;

        if (strchr(d->key[i], ':') == NULL) {
            foundsec++;

            if (foundsec > n)
                break;
        }
    }

    if (foundsec <= n) {
        return NULL;
    }

    return d->key[i];
}

void iniparser_dump(const dictionary *d, FILE *f)
{
    size_t i;

    if (d == NULL || f == NULL) return;

    for (i = 0; i < d->size; i++) {
        if (d->key[i] == NULL)
            continue;

        if (d->val[i] != NULL) {
            fprintf(f, "[%s]=[%s]\n", d->key[i], d->val[i]);
        } else {
            fprintf(f, "[%s]=UNDEF\n", d->key[i]);
        }
    }

    return;
}

static void escape_value(char *escaped, char *value)
{
    char c;
    int v = 0;
    int e = 0;

    if (!escaped || !value)
        return;

    while ((c = value[v]) != '\0') {
        if (c == '\\' || c == '"') {
            escaped[e] = '\\';
            e++;
        }

        escaped[e] = c;
        v++;
        e++;
    }

    escaped[e] = '\0';
}

void iniparser_dump_ini(const dictionary *d, FILE *f)
{
    size_t       i;
    size_t       nsec;
    const char *secname;
    char escaped[(ASCIILINESZ * 2) + 2] = "";

    if (d == NULL || f == NULL) return;

    nsec = iniparser_getnsec(d);

    if (nsec < 1) {
        /* No section in file: dump all keys as they are */
        for (i = 0; i < d->size; i++) {
            if (d->key[i] == NULL)
                continue;

            escape_value(escaped, d->val[i]);
            fprintf(f, "%s = \"%s\"\n", d->key[i], escaped);
        }

        return;
    }

    for (i = 0; i < nsec; i++) {
        secname = iniparser_getsecname(d, i);
        iniparser_dumpsection_ini(d, secname, f);
    }

    fprintf(f, "\n");
    return;
}

void iniparser_dumpsection_ini(const dictionary *d, const char *s, FILE *f)
{
    size_t  j;
    char    keym[ASCIILINESZ + 1];
    int     seclen;
    char escaped[(ASCIILINESZ * 2) + 2] = "";

    if (d == NULL || f == NULL) return;

    if (! iniparser_find_entry(d, s)) return;

    if (strlen(s) > sizeof(keym)) return;

    seclen  = (int)strlen(s);
    fprintf(f, "\n[%s]\n", s);
    sprintf(keym, "%s:", s);

    for (j = 0; j < d->size; j++) {
        if (d->key[j] == NULL)
            continue;

        if (!strncmp(d->key[j], keym, seclen + 1)) {
            escape_value(escaped, d->val[j]);
            fprintf(f, "%-30s = \"%s\"\n", d->key[j] + seclen + 1, escaped);
        }
    }

    fprintf(f, "\n");
    return;
}

int iniparser_getsecnkeys(const dictionary *d, const char *s)
{
    int     seclen, nkeys;
    char    keym[ASCIILINESZ + 1];
    size_t  j;

    nkeys = 0;

    if (d == NULL) return nkeys;

    if (! iniparser_find_entry(d, s)) return nkeys;

    seclen  = (int)strlen(s);
    strlwc(s, keym, sizeof(keym));
    keym[seclen] = ':';

    for (j = 0; j < d->size; j++) {
        if (d->key[j] == NULL)
            continue;

        if (!strncmp(d->key[j], keym, seclen + 1))
            nkeys++;
    }

    return nkeys;

}

const char **iniparser_getseckeys(const dictionary *d, const char *s,
                                  const char **keys)
{
    size_t i, j, seclen;
    char keym[ASCIILINESZ + 1];

    if (d == NULL || keys == NULL) return NULL;

    if (! iniparser_find_entry(d, s)) return NULL;

    seclen  = strlen(s);
    strlwc(s, keym, sizeof(keym));
    keym[seclen] = ':';

    i = 0;

    for (j = 0; j < d->size; j++) {
        if (d->key[j] == NULL)
            continue;

        if (!strncmp(d->key[j], keym, seclen + 1)) {
            keys[i] = d->key[j];
            i++;
        }
    }

    return keys;
}

const char *iniparser_getstring(const dictionary *d, const char *key,
                                const char *def)
{
    const char *lc_key;
    const char *sval;
    char tmp_str[ASCIILINESZ + 1];

    if (d == NULL || key == NULL)
        return def;

    lc_key = strlwc(key, tmp_str, sizeof(tmp_str));
    sval = dictionary_get(d, lc_key, def);
    return sval;
}

long int iniparser_getlongint(const dictionary *d, const char *key,
                              long int notfound)
{
    const char *str;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);

    if (str == NULL || str == INI_INVALID_KEY) return notfound;

    return strtol(str, NULL, 0);
}

int64_t iniparser_getint64(const dictionary *d, const char *key,
                           int64_t notfound)
{
    const char *str;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);

    if (str == NULL || str == INI_INVALID_KEY) return notfound;

    return strtoimax(str, NULL, 0);
}

uint64_t iniparser_getuint64(const dictionary *d, const char *key,
                             uint64_t notfound)
{
    const char *str;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);

    if (str == NULL || str == INI_INVALID_KEY) return notfound;

    return strtoumax(str, NULL, 0);
}

int iniparser_getint(const dictionary *d, const char *key, int notfound)
{
    return (int)iniparser_getlongint(d, key, notfound);
}

double iniparser_getdouble(const dictionary *d, const char *key,
                           double notfound)
{
    const char *str;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);

    if (str == NULL || str == INI_INVALID_KEY) return notfound;

    return atof(str);
}

int iniparser_getboolean(const dictionary *d, const char *key, int notfound)
{
    int          ret;
    const char *c;

    c = iniparser_getstring(d, key, INI_INVALID_KEY);

    if (c == NULL || c == INI_INVALID_KEY) return notfound;

    if (c[0] == 'y' || c[0] == 'Y' || c[0] == '1' || c[0] == 't' || c[0] == 'T') {
        ret = 1;
    } else if (c[0] == 'n' || c[0] == 'N' || c[0] == '0' || c[0] == 'f'
               || c[0] == 'F') {
        ret = 0;
    } else {
        ret = notfound;
    }

    return ret;
}

int iniparser_find_entry(const dictionary *ini, const char *entry)
{
    int found = 0;

    if (iniparser_getstring(ini, entry, INI_INVALID_KEY) != INI_INVALID_KEY) {
        found = 1;
    }

    return found;
}

int iniparser_set(dictionary *ini, const char *entry, const char *val)
{
    char tmp_key[ASCIILINESZ + 1] = {0};
    char tmp_val[ASCIILINESZ + 1] = {0};
    size_t len;

    if (val) {
        len = strlen(val);
        len = len > ASCIILINESZ ? ASCIILINESZ : len;
        memcpy(tmp_val, val, len);
        val = tmp_val;
    }

    return dictionary_set(ini, strlwc(entry, tmp_key, sizeof(tmp_key)), val);
}

void iniparser_unset(dictionary *ini, const char *entry)
{
    char tmp_str[ASCIILINESZ + 1];
    dictionary_unset(ini, strlwc(entry, tmp_str, sizeof(tmp_str)));
}

static void parse_quoted_value(char *value, char quote)
{
    char c;
    char *quoted;
    int q = 0, v = 0;
    int esc = 0;

    if (!value)
        return;

    quoted = strdup(value);

    if (!quoted) {
        iniparser_error_callback("iniparser: memory allocation failure\n");
        goto end_of_value;
    }

    while ((c = quoted[q]) != '\0') {
        if (!esc) {
            if (c == '\\') {
                esc = 1;
                q++;
                continue;
            }

            if (c == quote) {
                goto end_of_value;
            }
        }

        esc = 0;
        value[v] = c;
        v++;
        q++;
    }

end_of_value:
    value[v] = '\0';
    free(quoted);
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
static line_status iniparser_line(
    const char *input_line,
    char *section,
    char *key,
    char *value)
{
    line_status sta;
    char *line = NULL;
    size_t      len;
    int d_quote;

    line = strdup(input_line);
    len = strstrip(line);

    sta = LINE_UNPROCESSED;

    if (len < 1) {
        /* Empty line */
        sta = LINE_EMPTY;
    } else if (line[0] == '#' || line[0] == ';') {
        /* Comment line */
        sta = LINE_COMMENT;
    } else if (line[0] == '[' && line[len - 1] == ']') {
        /* Section name without opening square bracket */
        sscanf(line, "[%[^\n]", section);
        len = strlen(section);

        /* Section name without closing square bracket */
        if (section[len - 1] == ']') {
            section[len - 1] = '\0';
        }

        strstrip(section);
        strlwc(section, section, len);
        sta = LINE_SECTION;
    } else if ((d_quote = sscanf(line, "%[^=] = \"%[^\n]\"", key, value)) == 2
               ||  sscanf(line, "%[^=] = '%[^\n]'",   key, value) == 2) {
        /* Usual key=value with quotes, with or without comments */
        strstrip(key);
        strlwc(key, key, len);

        if (d_quote == 2)
            parse_quoted_value(value, '"');
        else
            parse_quoted_value(value, '\'');

        /* Don't strip spaces from values surrounded with quotes */
        sta = LINE_VALUE;
    } else if (sscanf(line, "%[^=] = %[^;#]", key, value) == 2) {
        /* Usual key=value without quotes, with or without comments */
        strstrip(key);
        strlwc(key, key, len);
        strstrip(value);

        /*
         * sscanf cannot handle '' or "" as empty values
         * this is done here
         */
        if (!strcmp(value, "\"\"") || (!strcmp(value, "''"))) {
            value[0] = 0;
        }

        sta = LINE_VALUE;
    } else if (sscanf(line, "%[^=] = %[;#]", key, value) == 2
               ||  sscanf(line, "%[^=] %[=]", key, value) == 2) {
        /*
         * Special cases:
         * key=
         * key=;
         * key=#
         */
        strstrip(key);
        strlwc(key, key, len);
        value[0] = 0;
        sta = LINE_VALUE;
    } else {
        /* Generate syntax error */
        sta = LINE_ERROR;
    }

    free(line);
    return sta;
}

dictionary *iniparser_load_file(FILE *in, const char *ininame)
{
    char line    [ASCIILINESZ + 1];
    char section [ASCIILINESZ + 1];
    char key     [ASCIILINESZ + 1];
    char tmp     [(ASCIILINESZ * 2) + 2];
    char val     [ASCIILINESZ + 1];

    int  last = 0;
    int  len;
    int  lineno = 0;
    int  errs = 0;
    int  mem_err = 0;

    dictionary *dict;

    dict = dictionary_new(0);

    if (!dict) {
        return NULL;
    }

    memset(line,    0, ASCIILINESZ);
    memset(section, 0, ASCIILINESZ);
    memset(key,     0, ASCIILINESZ);
    memset(val,     0, ASCIILINESZ);
    last = 0;

    while (fgets(line + last, ASCIILINESZ - last, in) != NULL) {
        lineno++;
        len = (int)strlen(line) - 1;

        if (len <= 0)
            continue;

        /* Safety check against buffer overflows */
        if (line[len] != '\n' && !feof(in)) {
            iniparser_error_callback(
                "iniparser: input line too long in %s (%d)\n",
                ininame,
                lineno);
            dictionary_del(dict);
            return NULL;
        }

        /* Get rid of \n and spaces at end of line */
        while ((len >= 0) &&
               ((line[len] == '\n') || (isspace((unsigned char)line[len])))) {
            line[len] = 0;
            len--;
        }

        if (len < 0) { /* Line was entirely \n and/or spaces */
            len = 0;
        }

        /* Detect multi-line */
        if (line[len] == '\\') {
            /* Multi-line value */
            last = len;
            continue;
        } else {
            last = 0;
        }

        switch (iniparser_line(line, section, key, val)) {
        case LINE_EMPTY:
        case LINE_COMMENT:
            break;

        case LINE_SECTION:
            mem_err = dictionary_set(dict, section, NULL);
            break;

        case LINE_VALUE:
            sprintf(tmp, "%s:%s", section, key);
            mem_err = dictionary_set(dict, tmp, val);
            break;

        case LINE_ERROR:
            iniparser_error_callback(
                "iniparser: syntax error in %s (%d):\n-> %s\n",
                ininame,
                lineno,
                line);
            errs++;
            break;

        default:
            break;
        }

        memset(line, 0, ASCIILINESZ);
        last = 0;

        if (mem_err < 0) {
            iniparser_error_callback("iniparser: memory allocation failure\n");
            break;
        }
    }

    if (errs) {
        dictionary_del(dict);
        dict = NULL;
    }

    return dict;
}

dictionary *iniparser_load(const char *ininame)
{
    FILE *in;
    dictionary *dict;

    if ((in = fopen(ininame, "r")) == NULL) {
        iniparser_error_callback("iniparser: cannot open %s\n", ininame);
        return NULL;
    }

    dict = iniparser_load_file(in, ininame);
    fclose(in);

    return dict;
}

void iniparser_freedict(dictionary *d)
{
    dictionary_del(d);
}
