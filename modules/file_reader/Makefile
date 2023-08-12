CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BUILD=./build


all: dirs builds

dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/fr.o $(DIR_BUILD)/libfr.a


$(DIR_BUILD):
	mkdir $(DIR_BUILD)


$(DIR_BUILD)/fr.o: ./src/fr.c ./src/fr.h
	$(CC) $(CFLAGS) -c ./src/fr.c -o $@

$(DIR_BUILD)/libfr.a: $(DIR_BUILD)/fr.o
	ar ruv $(DIR_BUILD)/libfr.a $(DIR_BUILD)/fr.o
	ranlib $(DIR_BUILD)/libfr.a



clean:
	rm -f $(DIR_BUILD)/*
	rm -f *.stackdump

