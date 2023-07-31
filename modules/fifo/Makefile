CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

SRC=/usr/local/src
INC=/usr/local/include
OBJ=/usr/local/obj
LIB=/usr/local/lib
SRCL=./src
BINL=./bin

all: ./build/fifo.o ./build/libfifo.a test



./build/fifo.o: ./src/fifo.c ./src/fifo.h
	$(CC) $(CFLAGS) -c ./src/fifo.c -o $@

./build/libfifo.a: ./build/fifo.o
	ar ruv ./build/libfifo.a ./build/fifo.o
	ranlib ./build/libfifo.a

test: ./build/libfifo.a
	make -C test

clean:
	rm -f ./build/*
	make -C test clean	
	rm -f *.stackdump

