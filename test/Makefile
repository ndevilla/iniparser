#
# iniparser tests Makefile
#

CC      = gcc
CFLAGS  = -g -I../src
LFLAGS  = -L.. -liniparser
AR	    = ar
ARFLAGS = rcv
RM      = rm -f


default: all

all: iniexample parse parse_array

iniexample: iniexample.c
	$(CC) $(CFLAGS) -o iniexample iniexample.c -I../src -L.. -liniparser

parse: parse.c
	$(CC) $(CFLAGS) -o parse parse.c -I../src -L.. -liniparser
	
parse_array: parse_array.c
	$(CC) $(CFLAGS) -o parse_array parse_array.c -I../src -L.. -liniparser

clean veryclean:
	$(RM) iniexample example.ini parse



