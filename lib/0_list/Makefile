CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib

all: deploy
lib:liblist.a

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

list.c: $(DIR_SRC)/list.c $(DIR_SRC)/list.h

list.o: list.c
	$(CC) $(CFLAGS) -I$(DIR_SRC) -c $(DIR_SRC)/list.c -o $@

liblist.a: list.o
	ar ruv liblist.a list.o
	ranlib liblist.a

deploy: liblist.a
	cp liblist.a $(DIR_LIB)/
	cp $(DIR_SRC)/list.h $(DIR_INC)/	

# Cleaning
clean: 
	rm list.o liblist.a
	rm -rf *.stackdump

