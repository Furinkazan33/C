CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_SRC=./src
DIR_BUILD=./build


all: dirs builds

dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/char.o $(DIR_BUILD)/libchar.a

$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)


$(DIR_BUILD)/char.o: $(DIR_SRC)/char.c $(DIR_SRC)/char.h
	$(CC) $(CFLAGS) -c $(DIR_SRC)/char.c -o $@

$(DIR_BUILD)/libchar.a: $(DIR_BUILD)/char.o
	ar ruv $(DIR_BUILD)/libchar.a $(DIR_BUILD)/char.o
	ranlib $(DIR_BUILD)/libchar.a


clean: 
	rm -f $(DIR_BUILD)/*
	rm -f *.stackdump

