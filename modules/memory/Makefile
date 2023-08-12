CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BIN=./bin


# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: dirs builds

dirs: $(DIR_BIN)
builds: $(DIR_BIN)/main


$(DIR_BIN):
	mkdir $(DIR_BIN)


$(DIR_BIN)/main: ./src/main.c ./src/mem.c
	$(CC) $(CFLAGS) ./src/main.c ./src/mem.c -o $@
	

clean: 
	rm ./$(DIR_BIN)/*

