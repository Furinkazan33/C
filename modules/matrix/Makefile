CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LBUILD=./build

SRC=/usr/local/src
LIB=/usr/local/lib
INC=/usr/local/include
RES=/usr/local/ressources


all: $(LBUILD)/matrix.o $(LBUILD)/libmatrix.a test


$(LBUILD)/matrix.o: ./src/matrix.c ./src/matrix.h
	$(CC) $(CFLAGS) -I ./src -c ./src/matrix.c -o $@

$(LBUILD)/libmatrix.a: $(LBUILD)/matrix.o
	ar ruv $(LBUILD)/libmatrix.a $(LBUILD)/matrix.o
	ranlib $(LBUILD)/libmatrix.a


test: $(LBUILD)/matrix.o
	make -C test



clean:
	rm -f $(LBUILD)/*
	make -C test clean
	rm -f *.stackdump

