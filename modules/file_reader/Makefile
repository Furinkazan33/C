CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LBUILD=./build
LBIN=./bin

SRC=/usr/local/src
LIB=/usr/local/lib
INC=/usr/local/include
RES=/usr/local/ressources


all: $(LBUILD)/fr.o


$(LBUILD)/fr.o:
	$(CC) $(CFLAGS) -c ./src/fr.c -o $@




clean:
	rm -f $(LBUILD)/*
	rm -f *.stackdump

