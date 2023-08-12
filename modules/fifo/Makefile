CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BUILD=./build

all: dirs builds test
dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/fifo.o $(DIR_BUILD)/libfifo.a


$(DIR_BUILD):
	mkdir -p $(DIR_BUILD)


$(DIR_BUILD)/fifo.o: ./src/fifo.c ./src/fifo.h
	$(CC) $(CFLAGS) -c ./src/fifo.c -o $@

$(DIR_BUILD)/libfifo.a: $(DIR_BUILD)/fifo.o
	ar ruv $(DIR_BUILD)/libfifo.a $(DIR_BUILD)/fifo.o
	ranlib $(DIR_BUILD)/libfifo.a

test: $(DIR_BUILD)/libfifo.a
	make -C test

clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean	
	rm -f *.stackdump

