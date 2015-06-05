### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Use includes from src/ directory
#
AM_CFLAGS += -I$(top_srcdir)/src



### Build test suite
#
TESTS            = AllTests
check_PROGRAMS   = AllTests
AllTests_SOURCES = \
	AllTests.c \
	CuTest.c \
	CuTest.h \
	test_dictionary.c \
	test_iniparser.c

AllTests_CFLAGS  = `echo $(AM_CFLAGS) | sed -e 's/-Werror//'`
