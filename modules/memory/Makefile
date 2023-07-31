CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=



# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: ./bin/main
	


# Objects dependencies
./bin/main: ./src/main.c ./src/mem.c
	$(CC) $(CFLAGS) ./src/main.c ./src/mem.c -o $@
	

clean: 
	rm ./bin/*

