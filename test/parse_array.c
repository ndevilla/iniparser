#include <stdio.h>
#include "../src/iniparser.h"

int main(int argc, char * argv[])
{
    dictionary *  ini;
    char       *  ini_name;
    int        *  integer_array;
    double     *  double_array;
    char       ** string_array;
    int          n, n_max;

    if (argc<2) {
        ini_name = "twisted-arrays.ini";
    } else {
        ini_name = argv[1] ;
    }

    ini = iniparser_load(ini_name);

    integer_array = iniparser_getint_array(ini, "Test:Integer", &n_max);
    for(n=0; n<n_max; n++)
    {
    	printf("%i = %i\n", n, integer_array[n]);
    }
    printf("\n");

    double_array = iniparser_getdouble_array(ini, "Test:Double", &n_max);
	for(n=0; n<n_max; n++)
	{
		printf("%i = %G\n", n, double_array[n]);
	}
	printf("\n");

    return 0 ;
}
