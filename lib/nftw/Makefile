CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_BIN=./bin

EXEC=main

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
main.c: $(DIR_SRC)/main.c


# Objects dependencies
main: main.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c -o $(DIR_BIN)/$@
	

# Cleaning
clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

