ACLOCAL_AMFLAGS = -I config

lib_LTLIBRARIES = libiniparser.la

libiniparser_la_LDFLAGS = -no-undefined -version-info @SHARED_VERSION_INFO@
libiniparser_la_SOURCES = dictionary.c  iniparser.c

include_HEADERS = dictionary.h  iniparser.h

install-exec-hook:
if NO_LA_FILES
	rm -f $(DESTDIR)$(libdir)/libiniparser.la
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libiniparser.pc

