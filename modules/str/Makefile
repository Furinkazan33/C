CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LSRC=./src
LBUILD=./build


all: ./build/str.o


./build/str.o: $(LSRC)/str.c $(LSRC)/str.h
	$(CC) $(CFLAGS) -c $(LSRC)/str.c -o $@


clean:
	rm -f $(LBUILD)/*

