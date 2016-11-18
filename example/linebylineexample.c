#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

void create_example_ini_file(void);

int line_callback(line_status lineStatus, const char * line, const char * section, const char * key, const char * val)
{
    printf("%d %s %s %s\n", lineStatus, section, key, val);
}

int main(int argc, char * argv[])
{
    int     status ;

    if (argc<2) {
        create_example_ini_file();
        iniparser_parse("example.ini", line_callback);
    } else {
        iniparser_parse(argv[1], line_callback);
    }
    return status ;
}

void create_example_ini_file(void)
{
    FILE    *   ini ;

    if ((ini=fopen("example.ini", "w"))==NULL) {
        fprintf(stderr, "iniparser: cannot create example.ini\n");
        return ;
    }

    fprintf(ini,
    "#\n"
    "# This is an example of ini file\n"
    "#\n"
    "\n"
    "[Pizza]\n"
    "\n"
    "Ham       = yes ;\n"
    "Mushrooms = TRUE ;\n"
    "Capres    = 0 ;\n"
    "Cheese    = Non ;\n"
    "\n"
    "\n"
    "[Wine]\n"
    "\n"
    "Grape     = Cabernet Sauvignon ;\n"
    "Year      = 1989 ;\n"
    "Country   = Spain ;\n"
    "Alcohol   = 12.5  ;\n"
    "\n");
    fclose(ini);
}
