CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib
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
libtb.a: $(DIR_LIB)/libtb.a
libconfig.a: $(DIR_LIB)/libconfig.a
main.c: $(DIR_SRC)/main.c

# Objects dependencies
main: main.c libtb.a libconfig.a
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c -L$(DIR_LIB) -lconfig -o $(DIR_BIN)/$@

test_main:
	$(DIR_BIN)/main ressources/init.cfg	> /dev/null  	|| echo "config ko"

# Cleaning
clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

