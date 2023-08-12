CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

EXEC=main

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: ./build ./build/main
	
./build:
	mkdir ./build

# Objects dependencies
./build/main: ./src/main.c
	$(CC) $(CFLAGS) ./src/main.c -o $@
	

# Cleaning
clean: 
	rm ./build/*
	rm -f *.stackdump

