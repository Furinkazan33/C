CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BUILD=./build


all: dirs builds tests
dirs: $(DIR_BUILD)
builds: $(DIR_BUILD)/libbtree.a $(DIR_BUILD)/btree.o
tests: test


$(DIR_BUILD):
	test -d $(DIR_BUILD) || mkdir -p $(DIR_BUILD)


$(DIR_BUILD)/btree.o: ./src/btree.c
	$(CC) $(CFLAGS) -I./src -c ./src/btree.c -o $@

$(DIR_BUILD)/libbtree.a: $(DIR_BUILD)/btree.o
	ar ruv $(DIR_BUILD)/libbtree.a $(DIR_BUILD)/btree.o
	ranlib $(DIR_BUILD)/libbtree.a

test: $(DIR_BUILD)/libbtree.a
	make -C test


clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean
	rm -f *.stackdump
	

