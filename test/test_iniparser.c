#include <stdio.h>

#include "CuTest.h"
#include "dictionary.h"

/* We need to directly insert the .c file in order to test the */
/* static functions as well */
#include "iniparser.c"

/* Tool function to create and populate a generic non-empty dictionary */
static dictionary * generate_dictionary(unsigned sections, unsigned entries_per_section)
{
    unsigned i, j ;
    dictionary * dic;
    char sec_name[32];
    char key_name[64];
    char key_value[32];

    dic = dictionary_new(sections + sections * entries_per_section);
    if (dic == NULL)
        return NULL;

    /* Insert the sections */
    for (i = 0; i < sections; ++i) {
        sprintf(sec_name, "sec%d", i);
        dictionary_set(dic, sec_name, "");
        for (j = 0; j < entries_per_section; ++j) {
            /* Populate the section with the entries */
            sprintf(key_name, "%s:key%d", sec_name, j);
            sprintf(key_value, "value-%d/%d", i, j);
            dictionary_set(dic, key_name, key_value);
        }
    }

    return dic;
}

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
    const char *stripped;
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
    char sec_name[32];
    dictionary *dic;

    /* NULL test */
    CuAssertIntEquals(tc, -1, iniparser_getnsec(NULL));

    /* Empty dictionary */
    dic = dictionary_new(10);
    CuAssertIntEquals(tc, 0, iniparser_getnsec(dic));
    dictionary_del(dic);

    /* Regular dictionary */
    dic = generate_dictionary(512, 0);
    CuAssertIntEquals(tc, 512, iniparser_getnsec(dic));

    /* Check after removing sections */
    for (i = 1; i < 512; ++i) {
        sprintf(sec_name, "sec%d", i);
        dictionary_unset(dic, sec_name);
        CuAssertIntEquals(tc, 512 - i, iniparser_getnsec(dic));
    }
    dictionary_del(dic);

    /* Mix sections and regular keys */
    dic = generate_dictionary(10, 512);
    CuAssertIntEquals(tc, 10, iniparser_getnsec(dic));
    dictionary_del(dic);
}

void Test_iniparser_getsecname(CuTest *tc)
{
    unsigned i;
    char sec_name[32];
    dictionary *dic;
    /* NULL test */
    CuAssertTrue(tc, iniparser_getsecname(NULL, 0) == NULL);

    /* Empty dictionary */
    dic = dictionary_new(10);
    CuAssertPtrEquals(tc, NULL, iniparser_getsecname(dic, 0));
    dictionary_del(dic);

    /* Sections without entries dictionary */
    dic = generate_dictionary(100, 0);
    for (i = 0; i < 100; ++i) {
        sprintf(sec_name, "sec%d", i);
        CuAssertStrEquals(tc, sec_name, iniparser_getsecname(dic, i));
    }
    dictionary_del(dic);

    /* Generic dictionary */
    dic = generate_dictionary(10, 100);
    for (i = 0; i < 10; ++i) {
        sprintf(sec_name, "sec%d", i);
        CuAssertStrEquals(tc, sec_name, iniparser_getsecname(dic, i));
    }
    dictionary_del(dic);
}

void Test_iniparser_getseckeys(CuTest *tc)
{
    unsigned i;
    char key_name[64];
    dictionary *dic;
    const char ** sections;
    /* NULL test */
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(NULL, NULL));
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(NULL, "dummy"));

    /* Empty dictionary */
    dic = dictionary_new(10);
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(dic, NULL));
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(dic, "dummy"));
    dictionary_del(dic);

    /* Generic dictionary */
    dic = generate_dictionary(100, 10);
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(dic, NULL));
    CuAssertPtrEquals(tc, NULL, iniparser_getseckeys(dic, "dummy"));
    sections = iniparser_getseckeys(dic, "sec42");
    CuAssertPtrNotNull(tc, sections);
    for (i = 0; i < 10; ++i) {
        sprintf(key_name, "sec42:key%d", i);
        CuAssertStrEquals(tc, key_name, sections[i]);
    }
    free(sections);
    dictionary_del(dic);
}

void Test_iniparser_getstring(CuTest *tc)
{
    dictionary *dic;
    /* NULL test */
    CuAssertPtrEquals(tc, NULL, iniparser_getstring(NULL, NULL, NULL));
    CuAssertPtrEquals(tc, NULL, iniparser_getstring(NULL, "dummy", NULL));

    /* Check the def return element */
    dic = dictionary_new(10);
    CuAssertPtrEquals(tc, NULL, iniparser_getstring(dic, "dummy", NULL));
    CuAssertStrEquals(tc, "def", iniparser_getstring(dic, NULL, "def"));
    CuAssertStrEquals(tc, "def", iniparser_getstring(dic, "dummy", "def"));
    dictionary_del(dic);

    /* Generic dictionary */
    dic = generate_dictionary(100, 10);
    CuAssertStrEquals(tc, "value-0/0",
                      iniparser_getstring(dic, "sec0:key0", NULL));
    CuAssertStrEquals(tc, "value-42/5",
                      iniparser_getstring(dic, "sec42:key5", NULL));
    CuAssertStrEquals(tc, "value-99/9",
                      iniparser_getstring(dic, "sec99:key9", NULL));
    dictionary_del(dic);
}

void Test_iniparser_getint(CuTest *tc)
{
    unsigned i;
    char key_name[64];
    dictionary *dic;
    const struct { int num; const char *value; } good_val[] = {
        { 0, "0" },
        { 1, "1" },
        { -1, "-1" },
        { 1000, "1000" },
        { 077, "077" },
        { -01000, "-01000" },
        { 0xFFFF, "0xFFFF" },
        { -0xFFFF, "-0xFFFF" },
        { 0x4242, "0x4242" },
        { 0, NULL} /* must be last */
    };
    const char *bad_val[] = {
        "",
        "notanumber",
        "0x",
        "k2000",
        " ",
        "0xG1"
    };
    /* NULL test */
    CuAssertIntEquals(tc, -42, iniparser_getint(NULL, NULL, -42));
    CuAssertIntEquals(tc, -42, iniparser_getint(NULL, "dummy", -42));

    /* Check the def return element */
    dic = dictionary_new(10);
    CuAssertIntEquals(tc, 42, iniparser_getint(dic, "dummy", 42));
    CuAssertIntEquals(tc, 0xFFFF, iniparser_getint(dic, NULL, 0xFFFF));
    CuAssertIntEquals(tc, -0xFFFF, iniparser_getint(dic, "dummy", -0xFFFF));
    dictionary_del(dic);

    /* Generic dictionary */
    dic = dictionary_new(10);
    for (i = 0; good_val[i].value != NULL; ++i) {
        sprintf(key_name, "int:value%d", i);
        dictionary_set(dic, key_name, good_val[i].value);
    }
    for (i = 0; good_val[i].value != NULL; ++i) {
        sprintf(key_name, "int:value%d", i);
        CuAssertIntEquals(tc, good_val[i].num,
                          iniparser_getint(dic, key_name, 0));
    }
    dictionary_del(dic);

    /* Test bad names */
    dic = dictionary_new(10);
    for (i = 0; i < sizeof (bad_val) / sizeof (char *); ++i) {
        sprintf(key_name, "int:bad%d", i);
        dictionary_set(dic, key_name, bad_val[i]);
    }
    for (i = 0; i < sizeof (bad_val) / sizeof (char *); ++i) {
        sprintf(key_name, "int:bad%d", i);
        CuAssertIntEquals(tc, 0,
                          iniparser_getint(dic, key_name, 0));
    }
    dictionary_del(dic);
}
