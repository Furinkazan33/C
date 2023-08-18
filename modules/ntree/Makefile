CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=ntree

LSRC=./src
LBUILD=./build


all: $(LBUILD) $(LBUILD)/$(TARGET).o $(LBUILD)/lib$(TARGET).a test

$(LBUILD):
	test -d $(LBUILD) || mkdir -p $(LBUILD)

$(LBUILD)/$(TARGET).o: $(LSRC)/$(TARGET).c $(LSRC)/$(TARGET).h
	$(CC) $(CFLAGS) -c $(LSRC)/$(TARGET).c -o $@

$(LBUILD)/lib$(TARGET).a: $(LBUILD)/$(TARGET).o
	ar ruv $(LBUILD)/lib$(TARGET).a $(LBUILD)/$(TARGET).o
	ranlib $(LBUILD)/lib$(TARGET).a

test: $(LBUILD)/lib$(TARGET).a
	make -C test


clean: 
	rm -f $(LBUILD)/*
	make -C test clean
	rm -f *.stackdump

