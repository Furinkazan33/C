CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LSRC=./src
LBUILD=./build


all: ./build/str.o ./build/libstr.a


./build/str.o: $(LSRC)/str.c $(LSRC)/str.h
	$(CC) $(CFLAGS) -c $(LSRC)/str.c -o $@

$(LBUILD)/libstr.a: $(LBUILD)/str.o
	ar ruv $(LBUILD)/libstr.a $(LBUILD)/str.o
	ranlib $(LBUILD)/libstr.a



clean:
	rm -f $(LBUILD)/*

