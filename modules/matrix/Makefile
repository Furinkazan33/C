CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BUILD=./build


all: dirs builds test
dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/matrix.o $(DIR_BUILD)/libmatrix.a


$(DIR_BUILD):
	test -d $(DIR_BUILD) || mkdir -p $(DIR_BUILD)


$(DIR_BUILD)/matrix.o: ./src/matrix.c ./src/matrix.h
	$(CC) $(CFLAGS) -I ./src -c ./src/matrix.c -o $@

$(DIR_BUILD)/libmatrix.a: $(DIR_BUILD)/matrix.o
	ar ruv $(DIR_BUILD)/libmatrix.a $(DIR_BUILD)/matrix.o
	ranlib $(DIR_BUILD)/libmatrix.a


test: $(DIR_BUILD)/matrix.o
	make -C test



clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean
	rm -f *.stackdump

