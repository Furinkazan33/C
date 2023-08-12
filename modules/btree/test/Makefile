CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src
OBJ=/usr/local/obj

all: compile tests


./bin/btree_person: ./src/btree_person.c $(SRC)/person.c ../build/libbtree.a
	$(CC) $(CFLAGS) -I $(INC) \
			./src/btree_person.c $(SRC)/person.c \
			-L ../build -lbtree \
			-L $(LIB) -lcjson \
			-o $@

./bin/btree_char: ./src/btree_char.c ../build/libbtree.a
	$(CC) $(CFLAGS) -I $(INC) \
			./src/btree_char.c \
			-L ../build -lbtree \
			-L $(LIB) -lcjson \
			-o $@

compile: ./bin/btree_person ./bin/btree_char

./log/btree_char.log: ./bin/btree_char
	./bin/btree_char > ./log/btree_char.log

./log/btree_person.log: ./bin/btree_person
	./bin/btree_person > ./log/btree_person.log


tests: ./log/btree_char.log ./log/btree_person.log


clean:
	rm -f ./bin/*	
	rm -f ./log/*	
	rm -f *.stackdump

