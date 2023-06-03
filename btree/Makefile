CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=$(DIR_SRC)/obj

EXEC=simple_example.exe example_with_static_struct.exe

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC)


# Sources dependencies
btree.c: $(DIR_INC)/btree.c $(DIR_INC)/btree.h
simple_example.c: $(DIR_SRC)/simple_example.c btree.c
example_with_static_struct.c: $(DIR_SRC)/example_with_static_struct.c btree.c

# Objects dependencies
btree.o: btree.c
	$(CC) $(CFLAGS) -I$(DIR_INC) -c $(DIR_INC)/btree.c -o $(DIR_OBJ)/$@
	
simple_example.exe: simple_example.c btree.o
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/simple_example.c $(DIR_OBJ)/btree.o -o $@
	
example_with_static_struct.exe: example_with_static_struct.c btree.o
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/example_with_static_struct.c $(DIR_OBJ)/btree.o -o $@

# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o *.o
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

