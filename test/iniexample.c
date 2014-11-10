#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "iniparser.h"

void create_example_ini_file(void);
int  parse_ini_file(char * ini_name);

int main(int argc, char * argv[])
{
    int     status ;

    if (argc<2) {
        create_example_ini_file();
        status = parse_ini_file("example.ini");
    } else {
        status = parse_ini_file(argv[1]);
    }
    return status ;
}

void create_example_ini_file(void)
{
    FILE    *   ini ;

    ini = fopen("example.ini", "w");
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


int parse_ini_file(char * ini_name)
{
    dictionary  *   ini ;

    int             rv;
    char            *tmp_name;
    FILE            *out;

    /* Some temporary variables to hold query results */
    int             b ;
    int             i ;
    double          d ;
    char        *   s ;

    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    iniparser_dump(ini, stderr);

    /* Get pizza attributes */
    printf("Pizza-------------:\n");

    b = iniparser_getboolean(ini, "pizza:ham", -1);
    printf("Ham:       [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:mushrooms", -1);
    printf("Mushrooms: [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:capres", -1);
    printf("Capres:    [%d]\n", b);
    b = iniparser_getboolean(ini, "pizza:cheese", -1);
    printf("Cheese:    [%d]\n", b);

    /* Get wine attributes */
    printf("Wine:\n");
    s = iniparser_getstring(ini, "wine:grape", NULL);
    printf("Grape:     [%s]\n", s ? s : "UNDEF");

    i = iniparser_getint(ini, "wine:year", -1);
    printf("Year:      [%d]\n", i);

    s = iniparser_getstring(ini, "wine:country", NULL);
    printf("Country:   [%s]\n", s ? s : "UNDEF");

    d = iniparser_getdouble(ini, "wine:alcohol", -1.0);
    printf("Alcohol:   [%g]\n", d);

    /* add fruit */
    rv = iniparser_set(ini, "Fruit:North:Apple", "12.5");
    if (rv) {
        printf("Add Apple data fail!\n");
    }
    rv = iniparser_set(ini, "Fruit:South:Orange", "6.5");
    if (rv) {
        printf("Add Orange data fail!\n");
    }

    /* export ini to tmpfile */
    tmp_name =  malloc(strlen("tmp_") + strlen(ini_name) + 1);
    if (!tmp_name) {
        printf("Make tmpfile name fail!\n");
        return -1;
    }
    sprintf(tmp_name, "tmp_%s", ini_name);

    if ((out = fopen(tmp_name, "w")) == NULL ) {
        printf("Open %s fail!\n", tmp_name);
        free(tmp_name);
        return -1;
    };

    iniparser_dump_ini(ini, out);
    fclose(out);

    iniparser_freedict(ini);
    return 0 ;
}


