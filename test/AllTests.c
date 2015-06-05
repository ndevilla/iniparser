/* This is auto-generated code. Edit at your own peril. */
#include <stdio.h>
#include <stdlib.h>

#include "CuTest.h"


extern void Test_xstrdup(CuTest*);
extern void Test_dictionary_grow(CuTest*);
extern void Test_dictionary_hash(CuTest*);
extern void Test_dictionary_growing(CuTest*);
extern void Test_dictionary_unset(CuTest*);
extern void Test_dictionary_dump(CuTest*);
extern void Test_iniparser_strlwc(CuTest*);
extern void Test_iniparser_strstrip(CuTest*);
extern void Test_iniparser_getnsec(CuTest*);
extern void Test_iniparser_getsecname(CuTest*);
extern void Test_iniparser_getseckeys(CuTest*);
extern void Test_iniparser_getstring(CuTest*);
extern void Test_iniparser_getint(CuTest*);
extern void Test_iniparser_getdouble(CuTest*);
extern void Test_iniparser_getboolean(CuTest*);
extern void Test_iniparser_line(CuTest*);
extern void Test_iniparser_load(CuTest*);
extern void Test_dictionary_wrapper(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();


    SUITE_ADD_TEST(suite, Test_xstrdup);
    SUITE_ADD_TEST(suite, Test_dictionary_grow);
    SUITE_ADD_TEST(suite, Test_dictionary_hash);
    SUITE_ADD_TEST(suite, Test_dictionary_growing);
    SUITE_ADD_TEST(suite, Test_dictionary_unset);
    SUITE_ADD_TEST(suite, Test_dictionary_dump);
    SUITE_ADD_TEST(suite, Test_iniparser_strlwc);
    SUITE_ADD_TEST(suite, Test_iniparser_strstrip);
    SUITE_ADD_TEST(suite, Test_iniparser_getnsec);
    SUITE_ADD_TEST(suite, Test_iniparser_getsecname);
    SUITE_ADD_TEST(suite, Test_iniparser_getseckeys);
    SUITE_ADD_TEST(suite, Test_iniparser_getstring);
    SUITE_ADD_TEST(suite, Test_iniparser_getint);
    SUITE_ADD_TEST(suite, Test_iniparser_getdouble);
    SUITE_ADD_TEST(suite, Test_iniparser_getboolean);
    SUITE_ADD_TEST(suite, Test_iniparser_line);
    SUITE_ADD_TEST(suite, Test_iniparser_load);
    SUITE_ADD_TEST(suite, Test_dictionary_wrapper);

    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
    CuStringDelete(output);
    CuSuiteDelete(suite);
}

int main(void)
{
    RunAllTests();
    return 0;
}
