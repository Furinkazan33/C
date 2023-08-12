CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=config

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src
OBJ=/usr/local/obj


all: test



./build/config.o: ./src/config.c ./src/config.h
	$(CC) $(CFLAGS) -c ./src/$(TARGET).c \
			-L $(LIB)/ -lcjson \
			-o $@

./build/libconfig.a: ./build/config.o
	ar ruv ./build/libconfig.a ./build/config.o
	ranlib ./build/libconfig.a

test: ./build/libconfig.a
	make -C test


clean:
	rm ./build/*
	make -C test clean


