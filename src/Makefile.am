### Include common Makefile configuration
#
include   $(top_srcdir)/build/Makefile.am.common



### Create main library, but without execve() wrappers
#
if SUBPACKAGE_ENABLED
noinst_LTLIBRARIES = \
    libiniparser.la
else
lib_LTLIBRARIES = \
    libiniparser.la
endif

libiniparser_la_SOURCES = \
    dictionary.c \
    dictionary.h \
    iniparser.c \
    iniparser.h
