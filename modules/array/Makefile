CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LBUILD=./build


all: $(LBUILD) $(LBUILD)/array.o $(LBUILD)/libarray.a test


$(LBUILD):
	mkdir $(LBUILD)

$(LBUILD)/array.o: ./src/array.c ./src/array.h
	$(CC) $(CFLAGS) -c ./src/array.c -o $@

$(LBUILD)/libarray.a: $(LBUILD)/array.o
	ar ruv $(LBUILD)/libarray.a $(LBUILD)/array.o
	ranlib $(LBUILD)/libarray.a


test: $(LBUILD)/array.o
	make -C test



clean:
	rm -f $(LBUILD)/*
	make -C test clean
	rm -f *.stackdump

