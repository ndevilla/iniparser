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

all: iniexample iniwrite parse

iniexample: iniexample.c
	$(CC) $(CFLAGS) -o iniexample iniexample.c -I../src -L.. -liniparser

iniexample: superimposeexample.c
	$(CC) $(CFLAGS) -o superimposeexample superimposeexample.c -I../src -L.. -liniparser

iniwrite: iniwrite.c
	$(CC) $(CFLAGS) -o iniwrite iniwrite.c -I../src -L.. -liniparser

parse: parse.c
	$(CC) $(CFLAGS) -o parse parse.c -I../src -L.. -liniparser

clean veryclean:
	$(RM) iniexample example.ini parse



