#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

char *def = "NoFounded\n";
char *sc  = "20";

int main(int argc, char * argv[])
{
    dictionary * ini ;
    char       * ini_name ;
    char       * val;
    int         rv;
    FILE       *out_fd;

    if (argc<2) {
        ini_name = "twisted.ini";
    } else {
        ini_name = argv[1] ;
    }

    out_fd = fopen("./parse.out.ini", "w");
    if (!out_fd) {
        printf("Error:open output file fail!\n");
        return 0;
    }

    ini = iniparser_load(ini_name);
    iniparser_dump(ini, stdout);

   /* set key test */
    printf("---------set & get test---------\n");
    rv = iniparser_set(ini, "log-format:test:test", sc);
    if (rv) {
        printf("Error:set test fail!\n");
    }
    printf("Set test:%s\n", sc);

    val = iniparser_getstring(ini, "log-format:test:test", def);
    printf("Get test:%s\n", val);
    iniparser_dump_ini(ini, out_fd);
    fclose(out_fd);

    

    iniparser_freedict(ini);

    return 0 ;
}

