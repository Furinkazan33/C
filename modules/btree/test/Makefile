CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log


all: dirs compile tests

dirs: $(DIR_BIN) $(DIR_LOG)
compile: $(DIR_BIN)/btree_person $(DIR_BIN)/btree_char
tests: $(DIR_LOG)/btree_char.log $(DIR_LOG)/btree_person.log


$(DIR_BIN):
	mkdir -p $(DIR_BIN)

$(DIR_LOG):
	mkdir -p $(DIR_LOG)


$(DIR_BIN)/btree_person: ./src/btree_person.c ./src/person.c ../build/libbtree.a
	$(CC) $(CFLAGS) ./src/btree_person.c ./src/person.c \
			-L ../build -lbtree \
			-o $@

$(DIR_BIN)/btree_char: ./src/btree_char.c ../build/libbtree.a
	$(CC) $(CFLAGS) ./src/btree_char.c \
			-L ../build -lbtree \
			-o $@


$(DIR_LOG)/btree_char.log: $(DIR_BIN)/btree_char
	$(DIR_BIN)/btree_char > $(DIR_LOG)/btree_char.log

$(DIR_LOG)/btree_person.log: $(DIR_BIN)/btree_person
	$(DIR_BIN)/btree_person > $(DIR_LOG)/btree_person.log



clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

