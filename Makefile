#
# iniparser Makefile
#
.PHONY: example libiniparser.so

# Compiler settings
CC      ?= gcc

CFLAGS  += -fPIC -Wall -Wextra -ansi -pedantic
ifndef DEBUG
ADDITIONAL_CFLAGS  ?= -O2
else
ADDITIONAL_CFLAGS  ?= -g
endif

CFLAGS += ${ADDITIONAL_CFLAGS}

# Ar settings to build the library
AR	    ?= ar
ARFLAGS = rcv

SHLD = ${CC} ${CFLAGS}
LDSHFLAGS = -shared -Wl,-Bsymbolic
LDFLAGS += -Wl,-rpath -Wl,/usr/lib -Wl,-rpath,/usr/lib

# .so.0 is for version 3.x, .so.1 is 4.x
SONAME_CURRENT ?= 1

# Set RANLIB to ranlib on systems that require it (Sun OS < 4, Mac OSX)
# RANLIB  = ranlib
RANLIB = true

RM      ?= rm -f


# Implicit rules

SUFFIXES = .o .c .h .a .so .sl

COMPILE.c	?= $(CC) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c

ifndef V
QUIET_AR	= @echo "AR	$@";
QUIET_CC	= @echo "CC	$@";
QUIET_LINK	= @echo "LINK	$@";
QUIET_RANLIB	= @echo "RANLIB	$@";
endif

.c.o:
	$(QUIET_CC)$(COMPILE.c) $(OUTPUT_OPTION) $<


SRCS = src/iniparser.c \
	   src/dictionary.c

OBJS = $(SRCS:.c=.o)


default:	libiniparser.a libiniparser.so

libiniparser.a:	$(OBJS)
	$(QUIET_AR)$(AR) $(ARFLAGS) $@ $^
	$(QUIET_RANLIB)$(RANLIB) $@

libiniparser.so:	$(OBJS)
	$(QUIET_LINK)$(SHLD) $(LDSHFLAGS) $(LDFLAGS) -o $@.$(SONAME_CURRENT) $(OBJS) \
		-Wl,-soname=`basename $@`.$(SONAME_CURRENT)

clean:
	$(RM) $(OBJS)
	@(cd test ; $(MAKE) clean)

veryclean:
	$(RM) $(OBJS) libiniparser.a libiniparser.so*
	rm -rf ./html ; mkdir html
	cd example ; $(MAKE) veryclean
	cd test ; $(MAKE) veryclean

docs:
	@(cd doc ; $(MAKE))
	
check: libiniparser.so
	@(cd test ; $(MAKE))

example: libiniparser.a
	@(cd example ; $(MAKE))	
