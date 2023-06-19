CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=obj

EXEC=main.exe

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
triangle.c: $(DIR_INC)/triangle.c $(DIR_INC)/triangle.h
main.c: triangle.c $(DIR_SRC)/main.c


# Objects dependencies
triangle.o: triangle.c
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $(DIR_INC)/triangle.c -o $(DIR_OBJ)/$@
	
$(EXEC): triangle.c main.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c $(DIR_INC)/triangle.c -o $@
	
# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

