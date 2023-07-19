CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib
DIR_SRC=./src
DIR_BIN=./bin

EXEC=main
TEST=test_main

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC) $(TEST)


# Sources dependencies
main.c: $(DIR_SRC)/main.c

# Objects dependencies
main: main.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c -L$(DIR_LIB) -ltb -o $(DIR_BIN)/$@

test_main:
	$(DIR_BIN)/main ressources/ici.txt	> /dev/null 	|| echo "tb ko"

# Cleaning
clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

