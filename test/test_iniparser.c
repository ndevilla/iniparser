#include <stdio.h>

#include "CuTest.h"
#include "dictionary.h"

/* We need to directly insert the .c file in order to test the */
/* static functions as well */
#include "iniparser.c"

void Test_iniparser_strlwc(CuTest *tc)
{
    /* First element in the array is the expected stripping result */
    const char *strings_empty[] = {
        "",
        "       "
    };
    const char *strings_test[] = {
        "test",
        "test ",
        "test          ",
        " test",
        "   test    "
    };
    char error_msg[128];
    unsigned i;

    /* NULL ptr as input */
    CuAssertPtrEquals(tc, NULL, strstrip(NULL));

    /* empty string */
    for (i = 0 ; i < sizeof (strings_empty) / sizeof (char *) ; ++i) {
        sprintf(error_msg, "Bad stripping : strstrip(\"%s\") ==> %s",
            strings_empty[i], strstrip(strings_empty[i]));
        CuAssertStrEquals_Msg(tc, error_msg, strstrip(strings_empty[i]), strings_empty[0]);
    }
    /* test string */
    for (i = 0 ; i < sizeof (strings_test) / sizeof (char *) ; ++i) {
        sprintf(error_msg, "Bad stripping : strstrip(\"%s\") ==> %s",
            strings_test[i], strstrip(strings_test[i]));
        CuAssertStrEquals_Msg(tc, error_msg, strstrip(strings_test[i]), strings_test[0]);
    }
}

void Test_iniparser_strstrip(CuTest *tc)
{
    /* First element in the array is the expected stripping result */
    const char *strings_empty[] = {
        "",
        "       "
    };
    const char *strings_test[] = {
        "test",
        "test ",
        "test          ",
        " test",
        "   test    "
    };
    char string_very_long[ASCIILINESZ * 2];
    char error_msg[128];
    char *stripped;
    unsigned i;

    /* NULL ptr as input */
    CuAssertPtrEquals(tc, NULL, strstrip(NULL));

    /* empty string */
    for (i = 0 ; i < sizeof (strings_empty) / sizeof (char *) ; ++i) {
        stripped = strstrip(strings_empty[i]);
        sprintf(error_msg, "Bad stripping : strstrip(\"%s\") ==> \"%s\"",
            strings_empty[i], stripped);
        CuAssertStrEquals_Msg(tc, error_msg, stripped, strings_empty[0]);
    }

    /* test string */
    for (i = 0 ; i < sizeof (strings_test) / sizeof (char *) ; ++i) {
        stripped = strstrip(strings_test[i]);
        sprintf(error_msg, "Bad stripping : strstrip(\"%s\") ==> \"%s\"",
            strings_test[i], stripped);
        CuAssertStrEquals_Msg(tc, error_msg, stripped, strings_test[0]);
    }

    /* test a overflowing string */
    memset(string_very_long, '#', sizeof (string_very_long));
    string_very_long[sizeof (string_very_long) - 1] = '\0';
    CuAssertPtrEquals(tc, NULL, strstrip(string_very_long));
}

void Test_iniparser_getnsec(CuTest *tc)
{
    int i;
    int j;
    char sec_name[32];
    char key_name[64];
    dictionary *dic;

    /* NULL test */
    CuAssertTrue(tc, iniparser_getnsec(NULL) == -1);

    /* Empty dictionary */
    dic = dictionary_new(10);
    CuAssertTrue(tc, iniparser_getnsec(dic) == 0);

    /* Regular dictionary */
    for (i = 1 ; i < 512; ++i) {
        sprintf(sec_name, "sec%d", i);
        CuAssertTrue(tc, dictionary_set(dic, sec_name, "") == 0);
        CuAssertTrue(tc, iniparser_getnsec(dic) == i);
    }

    /* Check after removing sections */
    for (i = 1; i < 512; ++i) {
        sprintf(sec_name, "sec%d", i);
        dictionary_unset(dic, sec_name);
        CuAssertTrue(tc, iniparser_getnsec(dic) == 511 - i);
    }

    /* Mix sections and regular keys */
    for (i = 1; i < 512; ++i) {
        dictionary_set(dic, sec_name, "");
        for (j = 1; j < 10; ++j) {
            sprintf(key_name, "%s:key%d", sec_name, j);
            CuAssertTrue(tc, dictionary_set(dic, key_name, "dummy") == 0);
        }
        sprintf(sec_name, "sec%d", i);
        CuAssertTrue(tc, iniparser_getnsec(dic) == i);
    }
}

void Test_iniparser_getsecname(CuTest *tc)
{
    dictionary *dic;
    /* NULL test */
    CuAssertTrue(tc, iniparser_getsecname(NULL, 0) == NULL);

    /* Empty dictionary */
    dic = dictionary_new(10);
    CuAssertTrue(tc, iniparser_getsecname(dic, 0) == NULL);

    /* Populate the dictionary */

    /* TODO... */
}
