#include <stdio.h>
#include <stdlib.h>

#include "iniparser.h"

/*
 * This example demonstrates parsing ini data that is already held in a
 * memory buffer (a null-terminated string) using iniparser_load_buffer().
 */

static const char ini_buffer[] =
    "#\n"
    "# This ini data lives in a memory buffer\n"
    "#\n"
    "\n"
    "[Pizza]\n"
    "Ham       = yes ;\n"
    "Mushrooms = TRUE ;\n"
    "Capres    = 0 ;\n"
    "Cheese    = Non ;\n"
    "\n"
    "[Wine]\n"
    "Grape     = Cabernet Sauvignon ;\n"
    "Year      = 1989 ;\n"
    "Country   = Spain ;\n"
    "Alcohol   = 12.5 ;\n";

int main(void)
{
#ifdef INIPARSER_HAVE_LOAD_BUFFER
    dictionary *ini;

    ini = iniparser_load_buffer(ini_buffer);

    if (ini == NULL) {
        fprintf(stderr, "cannot parse ini buffer\n");
        return EXIT_FAILURE;
    }

    printf("Parsed ini buffer:\n\n");
    iniparser_dump(ini, stdout);

    printf("\nPizza has ham    : %d\n",
           iniparser_getboolean(ini, "pizza:ham", 0));
    printf("Wine year        : %d\n",
           iniparser_getint(ini, "wine:year", -1));
    printf("Wine alcohol     : %g\n",
           iniparser_getdouble(ini, "wine:alcohol", -1.0));

    iniparser_freedict(ini);
    return EXIT_SUCCESS;
#else
    (void)ini_buffer;
    fprintf(stderr,
            "iniparser_load_buffer() is not available on this platform "
            "(fmemopen() not exposed).\n"
            "Define an appropriate feature test macro "
            "(e.g. _POSIX_C_SOURCE=200809L) to enable it.\n");
    return EXIT_FAILURE;
#endif
}