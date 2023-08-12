CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_BUILD=./build


all: dirs builds

dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/str.o $(DIR_BUILD)/libstr.a

$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)

$(DIR_BUILD)/str.o: $(DIR_SRC)/str.c $(DIR_SRC)/str.h
	$(CC) $(CFLAGS) -c $(DIR_SRC)/str.c -o $@

$(DIR_BUILD)/libstr.a: $(DIR_BUILD)/str.o
	ar ruv $(DIR_BUILD)/libstr.a $(DIR_BUILD)/str.o
	ranlib $(DIR_BUILD)/libstr.a



clean:
	rm -f $(DIR_BUILD)/*

