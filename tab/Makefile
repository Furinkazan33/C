CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=$(DIR_SRC)/obj

EXEC=main.o

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
tab.c: $(DIR_INC)/tab.c $(DIR_INC)/tab.h
main.c: $(DIR_SRC)/main.c tab.c


# Objects dependencies
tab.o: tab.c
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $(DIR_INC)/tab.c -o $(DIR_OBJ)/$@
	
main.o: main.c tab.o
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c $(DIR_OBJ)/tab.o -o $@
	

# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

