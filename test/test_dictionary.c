#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"

/* We need to directly insert the .c file in order to test the */
/* static functions as well */
#include "dictionary.c"

void Test_xstrdup(CuTest *tc)
{
    size_t i;
    char *dup_str;
    const char *strings[] = {
        "",
        "test",
        " "
    };
    char *string_very_long;

    /* NULL test */
    CuAssertPtrEquals(tc, NULL, xstrdup(NULL));

    for (i = 0 ; i < sizeof(strings) / sizeof(char *) ; ++i) {
        dup_str = xstrdup(strings[i]);
        CuAssertStrEquals(tc, strings[i], dup_str);
        free(dup_str);
    }

    /* test a overflowing string */
    string_very_long = malloc(10 * 1024);
    memset(string_very_long, '#', 10 * 1024);
    string_very_long[10 * 1024 - 1] = '\0';
    dup_str = xstrdup(string_very_long);
    CuAssertStrEquals(tc, string_very_long, dup_str);

    free(string_very_long);
    free(dup_str);
}

void Test_dictionary_grow(CuTest *tc)
{
    unsigned i;
    dictionary *dic;

    dic = dictionary_new(DICTMINSZ);
    CuAssertPtrNotNull(tc, dic);
    CuAssertIntEquals(tc, 0, dic->n);
    CuAssertIntEquals(tc, DICTMINSZ, dic->size);

    for (i = 1 ; i < 10 ; ++i) {
        CuAssertIntEquals(tc, 0, dictionary_grow(dic));
        CuAssertIntEquals(tc, 0, dic->n);
        CuAssertIntEquals(tc, (1 << i) * DICTMINSZ, dic->size);
    }
}

void Test_dictionary_hash(CuTest *tc)
{
    /* NULL test */
    CuAssertTrue(tc, dictionary_hash(NULL) == 0);
}

void Test_dictionary_growing(CuTest *tc)
{
    int i, j;
    char sec_name[32];
    char key_name[64];
    dictionary *dic;

    dic = dictionary_new(DICTMINSZ);
    CuAssertPtrNotNull(tc, dic);
    CuAssertIntEquals(tc, 0, dic->n);

    /* Makes the dictionary grow */
    for (i = 1 ; i < 101; ++i) {
        sprintf(sec_name, "sec%d", i);
        CuAssertIntEquals(tc, 0, dictionary_set(dic, sec_name, ""));
        for (j = 1 ; j < 11; ++j) {
            sprintf(key_name, "%s:key%d", sec_name, j);
            CuAssertIntEquals(tc, 0, dictionary_set(dic, key_name, "dummy_value"));
            CuAssertIntEquals(tc, i + (i - 1) * 10 + j, dic->n);
        }
    }

    /* Shrink the dictionary */
    for (i = 100 ; i > 0; --i) {
        sprintf(sec_name, "sec%d", i);
        for (j = 10 ; j > 0; --j) {
            sprintf(key_name, "%s:key%d", sec_name, j);
            dictionary_unset(dic, key_name);
        }
        dictionary_unset(dic, sec_name);
        CuAssertIntEquals(tc, (i - 1) * (11), dic->n);
    }

    dictionary_del(dic);
}

static char *get_dump(dictionary *d)
{
    FILE *fd;
    char *dump_buff;
    int dump_size;

    /* Dump the dictionary in temporary file */
    fd = tmpfile();
    if (fd == NULL)
        return NULL;
    dictionary_dump(d, fd);

    /* Retrieve the dump file */
    dump_size = ftell(fd);
    if (dump_size == -1) {
        fclose(fd);
        return NULL;
    }
    rewind(fd);

    dump_buff = calloc(1, dump_size + 1);
    if (dump_buff == NULL) {
        fclose(fd);
        return NULL;
    }
    fread(dump_buff, 1, dump_size, fd);

    fclose(fd);
    return dump_buff;
}

void Test_dictionary_dump(CuTest *tc)
{
    int i, j;
    char sec_name[32];
    char key_name[64];
    dictionary *dic;
    char *dump_buff;
    const char dump_real[] = "\
                sec1\t[]\n\
           sec1:key1\t[dummy_value]\n\
           sec1:key2\t[dummy_value]\n\
           sec1:key3\t[dummy_value]\n\
           sec1:key4\t[dummy_value]\n\
                sec2\t[]\n\
           sec2:key1\t[dummy_value]\n\
           sec2:key2\t[dummy_value]\n\
           sec2:key3\t[dummy_value]\n\
           sec2:key4\t[dummy_value]\n\
";

    dic = dictionary_new(DICTMINSZ);
    CuAssertPtrNotNull(tc, dic);

    /* Try with empty dictionary first */
    dump_buff = get_dump(dic);
    CuAssertStrEquals(tc, "empty dictionary\n", dump_buff);
    free(dump_buff);

    /* Populate the dictionary */
    for (i = 1 ; i < 3; ++i) {
        sprintf(sec_name, "sec%d", i);
        dictionary_set(dic, sec_name, "");
        for (j = 1 ; j < 5; ++j) {
            sprintf(key_name, "%s:key%d", sec_name, j);
            dictionary_set(dic, key_name, "dummy_value");
        }
    }

    /* Check the dump file */
    dump_buff = get_dump(dic);
    CuAssertStrEquals(tc, dump_real, dump_buff);
    free(dump_buff);

    dictionary_del(dic);
}