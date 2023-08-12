CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=config

LIB=/usr/local/lib

DIR_BUILD=./build


all: dirs builds test

dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/config.o $(DIR_BUILD)/libconfig.a


$(DIR_BUILD):
	mkdir $(DIR_BUILD)


$(DIR_BUILD)/config.o: ./src/config.c ./src/config.h
	$(CC) $(CFLAGS) -c ./src/$(TARGET).c \
			-L $(LIB)/ -lcjson \
			-o $@

$(DIR_BUILD)/libconfig.a: $(DIR_BUILD)/config.o
	ar ruv $(DIR_BUILD)/libconfig.a $(DIR_BUILD)/config.o
	ranlib $(DIR_BUILD)/libconfig.a

test: $(DIR_BUILD)/libconfig.a
	make -C test


clean:
	rm $(DIR_BUILD)/*
	make -C test clean


