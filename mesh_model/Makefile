CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=obj

EXEC=main

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
object.c: $(DIR_SRC)/object.c $(DIR_SRC)/object.h
load_save.c: object.c $(DIR_SRC)/load_save.c


# Objects dependencies
$(EXEC): object.c load_save.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/load_save.c $(DIR_SRC)/object.c -o $@
	
# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o
	rm -rf *.obj.sav
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

