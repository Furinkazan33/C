CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

SRC=/usr/local/src
INC=/usr/local/include
LIB=/usr/local/lib
OBJ=/usr/local/obj


all: ./build/libbtree.a ./build/btree.o test


./build/btree.o: ./src/btree.c
	$(CC) $(CFLAGS) -I./src -c ./src/btree.c -o $@

./build/libbtree.a: ./build/btree.o
	ar ruv ./build/libbtree.a ./build/btree.o
	ranlib ./build/libbtree.a

test: ./build/libbtree.a
	make -C test


clean:
	rm -f ./build/*
	make -C test clean
	rm -f *.stackdump
	

