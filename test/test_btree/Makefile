CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_BIN=./bin

EXEC=btree_char btree_person

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
person.c: $(DIR_INC)/person.c $(DIR_INC)/person.h
btree.c: $(DIR_INC)/btree.c $(DIR_INC)/btree.h
btree_char.c: $(DIR_SRC)/btree_char.c
btree_person.c: $(DIR_SRC)/btree_person.c

# Objects dependencies
btree_char: btree_char.c btree.c
	$(CC) -DDATA_TYPE="char" $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/btree_char.c $(DIR_INC)/btree.c -o $(DIR_BIN)/$@

btree_person: btree_person.c btree.c person.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/btree_person.c $(DIR_INC)/person.c $(DIR_INC)/btree.c -o $(DIR_BIN)/$@

# Cleaning
clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

