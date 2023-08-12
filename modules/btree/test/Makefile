CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src


DIR_BIN=./bin
DIR_LOG=./log


all: dirs compile tests

dirs: $(DIR_BIN) $(DIR_LOG)
compile: $(DIR_BIN)/btree_person $(DIR_BIN)/btree_char
tests: $(DIR_LOG)/btree_char.log $(DIR_LOG)/btree_person.log


$(DIR_BIN):
	mkdir $(DIR_BIN)

$(DIR_LOG):
	mkdir $(DIR_LOG)


$(DIR_BIN)/btree_person: ./src/btree_person.c $(SRC)/person.c ../build/libbtree.a
	$(CC) $(CFLAGS) -I $(INC) \
			./src/btree_person.c $(SRC)/person.c \
			-L ../build -lbtree \
			-L $(LIB) -lcjson \
			-o $@

$(DIR_BIN)/btree_char: ./src/btree_char.c ../build/libbtree.a
	$(CC) $(CFLAGS) -I $(INC) \
			./src/btree_char.c \
			-L ../build -lbtree \
			-L $(LIB) -lcjson \
			-o $@


$(DIR_LOG)/btree_char.log: $(DIR_BIN)/btree_char
	$(DIR_BIN)/btree_char > $(DIR_LOG)/btree_char.log

$(DIR_LOG)/btree_person.log: $(DIR_BIN)/btree_person
	$(DIR_BIN)/btree_person > $(DIR_LOG)/btree_person.log



clean:
	rm -f $(DIR_BIN)/*	
	rm -f $(DIR_LOG)/*	
	rm -f *.stackdump

