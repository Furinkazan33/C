CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

SRCL=./src
SRC=/usr/local/src
INC=/usr/local/include
LIB=/usr/local/lib

all: test


./build/list.o: ./src/list.c ./src/list.h
	$(CC) $(CFLAGS) -c ./src/list.c -o $@

./build/liblist.a: ./build/list.o
	ar ruv ./build/liblist.a ./build/list.o
	ranlib ./build/liblist.a

test: ./build/liblist.a
	make -C test

clean:
	rm -f ./build/*
	make -C test clean

