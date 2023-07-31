CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


LSRC=./src
LBUILD=./build
LTEST=./test


all: $(LBUILD)/char.o

CHAR_C=$(LSRC)/char.c
CHAR_H=$(LSRC)/char.h
CHAR=$(CHAR_C) $(CHAR_H)

$(LBUILD)/char.o: $(CHAR)
	$(CC) $(CFLAGS) -c $(CHAR_C) -o $@



clean: 
	rm -f *.stackdump

