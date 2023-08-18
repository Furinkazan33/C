CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_BUILD=./build


all: $(DIR_BUILD) builds test

builds: $(DIR_BUILD)/list.o $(DIR_BUILD)/liblist.a  

$(DIR_BUILD):
	test -d $(DIR_BUILD) || mkdir -p $(DIR_BUILD)


$(DIR_BUILD)/list.o: $(DIR_SRC)/list.c $(DIR_SRC)/list.h
	$(CC) $(CFLAGS) -c $(DIR_SRC)/list.c -o $@

$(DIR_BUILD)/liblist.a: $(DIR_BUILD)/list.o
	ar ruv $(DIR_BUILD)/liblist.a $(DIR_BUILD)/list.o
	ranlib $(DIR_BUILD)/liblist.a

test: $(DIR_BUILD)/liblist.a
	make -C test

clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean

