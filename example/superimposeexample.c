#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "iniparser.h"

struct pizza {
    bool ham;
    bool mushrooms;
    bool capres;
    bool cheese;
};

void create_example_ini_files(void);
int parse_ini_file(struct pizza *pizza, char * ini_name);

int main(void)
{
    struct pizza pizza;
    int     status ;

    create_example_ini_files();
    status = parse_ini_file(&pizza, "global.ini");
    if(status < 0) {
        return status;
    }
    status = parse_ini_file(&pizza, "user.ini");
    if(status < 0) {
        return status;
    }

    printf("Pizza:\n");
    printf("Ham:       [%d]\n", pizza.ham);
    printf("Mushrooms: [%d]\n", pizza.mushrooms);
    printf("Capres:    [%d]\n", pizza.capres);
    printf("Cheese:    [%d]\n", pizza.cheese);

}

void create_example_ini_files(void)
{
    FILE    *   ini ;

    if ((ini=fopen("global.ini", "w"))==NULL) {
        fprintf(stderr, "iniparser: cannot create global.ini\n");
        return ;
    }

    fprintf(ini,
            "#\n"
            "# This is an example of global ini file\n"
            "#\n"
            "\n"
            "[Pizza]\n"
            "\n"
            "Ham       = yes ;\n"
            "Mushrooms = TRUE ;\n"
            "Capres    = 0 ;\n"
            "Cheese    = Non ;\n"
            "\n");
    fclose(ini);

    if ((ini=fopen("user.ini", "w"))==NULL) {
        fprintf(stderr, "iniparser: cannot create user.ini\n");
        return ;
    }

    fprintf(ini,
            "#\n"
            "# This is an example of user ini file\n"
            "#\n"
            "\n"
            "[Pizza]\n"
            "\n"
            "Ham       = no ;\n"
            "Mushrooms = TRUE ;\n"
            "Cheese    = Non ;\n"
            "\n");
    fclose(ini);
}

int parse_ini_file(struct pizza *pizza, char * ini_name)
{
    dictionary  *   ini ;

    /* Some temporary variables to hold query results */
    int             b ;
    int             i ;
    double          d ;
    const char  *   s ;

    if (pizza==NULL) {
        fprintf(stderr, "invalid parameter\n");
        return -1 ;
    }
    ini = iniparser_load(ini_name);
    if (ini==NULL) {
        fprintf(stderr, "cannot parse file: %s\n", ini_name);
        return -1 ;
    }
    iniparser_dump(ini, stderr);

    /* Get pizza attributes */
    b = iniparser_getboolean(ini, "pizza:ham", -1);
    if(b >= 0)
        pizza->ham = iniparser_getboolean(ini, "pizza:ham", -1);
    b = iniparser_getboolean(ini, "pizza:mushrooms", -1);
    if(b >= 0)
        pizza->mushrooms = iniparser_getboolean(ini, "pizza:mushrooms", -1);
    b = iniparser_getboolean(ini, "pizza:capres", -1);
    if(b >= 0)
        pizza->capres = iniparser_getboolean(ini, "pizza:capres", -1);
    b = iniparser_getboolean(ini, "pizza:cheese", -1);
    if(b >= 0)
        pizza->cheese = iniparser_getboolean(ini, "pizza:cheese", -1);

    iniparser_freedict(ini);
    return 0 ;
}
