CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=$(DIR_SRC)/obj

EXEC=simple_example.o

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
ntree.c: $(DIR_INC)/ntree.c $(DIR_INC)/ntree.h
simple_example.c: $(DIR_SRC)/simple_example.c ntree.c


# Objects dependencies
ntree.o: ntree.c
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $(DIR_INC)/ntree.c -o $(DIR_OBJ)/$@
	
simple_example.o: simple_example.c ntree.o
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/simple_example.c $(DIR_OBJ)/ntree.o -o $@
	

# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

