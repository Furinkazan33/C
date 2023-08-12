CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LBUILD=./build
LBIN=./bin

SRC=/usr/local/src
LIB=/usr/local/lib
INC=/usr/local/include
RES=/usr/local/ressources


all: $(LBUILD)/fr.o $(LBUILD)/libfr.a


$(LBUILD)/fr.o: ./src/fr.c ./src/fr.h
	$(CC) $(CFLAGS) -c ./src/fr.c -o $@

./build/libfr.a: ./build/fr.o
	ar ruv ./build/libfr.a ./build/fr.o
	ranlib ./build/libfr.a



clean:
	rm -f $(LBUILD)/*
	rm -f *.stackdump
