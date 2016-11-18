#
# iniparser tests Makefile
#

CC      ?= gcc
CFLAGS  += -g -I../src
LFLAGS  += -L.. -liniparser
AR	    ?= ar
ARFLAGS += rcv
RM      ?= rm -f


default: all

all: iniexample parse linebylineexample

iniexample: iniexample.c
	$(CC) $(CFLAGS) -o iniexample iniexample.c -I../src -L.. -liniparser

parse: parse.c
	$(CC) $(CFLAGS) -o parse parse.c -I../src -L.. -liniparser

linebylineexample: linebylineexample.c
	$(CC) $(CFLAGS) -o linebylineexample linebylineexample.c -I../src -L.. -liniparser

clean veryclean:
	$(RM) iniexample example.ini parse linebylineexample



