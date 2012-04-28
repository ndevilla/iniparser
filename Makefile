#
# iniparser Makefile
#

prefix ?= /usr/local

# Compiler settings
CC      = gcc
CFLAGS  = -O2 -fPIC -Wall -ansi -pedantic

# Ar settings to build the library
AR	    = ar
ARFLAGS = rcv

SHLD = ${CC} ${CFLAGS}
LDSHFLAGS = -shared
LDFLAGS =

# Set RANLIB to ranlib on systems that require it (Sun OS < 4, Mac OSX)
# RANLIB  = ranlib
RANLIB = true

RM      = rm -f


# Implicit rules

SUFFIXES = .o .c .h .a .so .sl

COMPILE.c=$(CC) $(CFLAGS) -c
.c.o:
	@(echo "compiling $< ...")
	@($(COMPILE.c) -o $@ $<)


SRCS = src/iniparser.c \
	   src/dictionary.c

OBJS = $(SRCS:.c=.o)


default:	libiniparser.a libiniparser.so

libiniparser.a:	$(OBJS)
	@($(AR) $(ARFLAGS) libiniparser.a $(OBJS))
	@($(RANLIB) libiniparser.a)

libiniparser.so:	$(OBJS)
	@$(SHLD) $(LDSHFLAGS) -o $@.0 $(OBJS) $(LDFLAGS) \
		-Wl,-soname=`basename $@`.0

clean:
	$(RM) $(OBJS)

veryclean:
	$(RM) $(OBJS) libiniparser.a libiniparser.so*
	rm -rf ./html ; mkdir html
	cd test ; $(MAKE) veryclean

docs:
	@(cd doc ; $(MAKE))
	
check:
	@(cd test ; $(MAKE))

install: libiniparser.so
	mkdir -p $(DESTDIR)$(prefix)/lib
	mkdir -p $(DESTDIR)$(prefix)/include
	cp libiniparser.so.0 $(DESTDIR)$(prefix)/lib
	ln -s $(DESTDIR)$(prefix)/lib/libiniparser.so.0 $(DESTDIR)$(prefix)/lib/libiniparser.so
	cp libiniparser.a $(DESTDIR)$(prefix)/lib
	cp src/*.h $(DESTDIR)$(prefix)/include
	mkdir -p $(DESTDIR)/usr/lib/pkgconfig/
	cp iniparser.pc $(DESTDIR)/usr/lib/pkgconfig/
	echo echo $(prefix) | sed s/\\//\\\\\\//g
	sed -i s/@prefix@/$(shell echo $(prefix) | sed s/\\//\\\\\\\\\\//g)/g $(DESTDIR)/usr/lib/pkgconfig/iniparser.pc

