### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Use includes from src/ directory
#
AM_CFLAGS += -I$(top_srcdir)/src



### Defaults
#
noinst_PROGRAMS =
LDADD           = ../src/libiniparser.la



### Append things to be done
#
noinst_PROGRAMS    += iniexample
iniexample_SOURCES  = iniexample.c

noinst_PROGRAMS    += parse
parse_SOURCES       = parse.c



### Remove test files
#
clean-local veryclean-local:
	$(RM) iniexample example.ini parse
