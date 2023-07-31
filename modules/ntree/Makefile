CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=ntree

LSRC=./src
LBUILD=./build

SRC=/usr/local/src
INC=/usr/local/include
LIB=/usr/local/lib

all: $(LBUILD)/$(TARGET).o $(LBUILD)/lib$(TARGET).a test


$(LBUILD)/$(TARGET).o: $(LSRC)/$(TARGET).c $(LSRC)/$(TARGET).h
	$(CC) $(CFLAGS) -c $(LSRC)/$(TARGET).c -o $@

$(LBUILD)/lib$(TARGET).a: $(LBUILD)/$(TARGET).o
	ar ruv $(LBUILD)/lib$(TARGET).a $(LBUILD)/$(TARGET).o
	ranlib $(LBUILD)/lib$(TARGET).a

test: $(LBUILD)/lib$(TARGET).a
	make -C test


clean: 
	rm $(LBUILD)/*
	make -C test clean
	rm -f *.stackdump

