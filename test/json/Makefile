CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_LIB=/usr/local/lib
DIR_INC=/usr/local/include
DIR_SRC=/usr/local/src
DIR_BIN=./bin

all: main

# Sources
main.c: ./src/main.c

# Objects
main: main.c $(DIR_INC)/cJSON.h
	$(CC) $(CFLAGS) -I$(DIR_INC) ./src/main.c $(DIR_SRC)/person.c -L $(DIR_LIB) -lcjson -o $(DIR_BIN)/$@

test: main
	$(DIR_BIN)/main

clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

