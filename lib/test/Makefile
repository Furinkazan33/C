CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_SRC=./src
DIR_SRC_TEST=$(DIR_SRC)/test
DIR_DEV=./dev
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=$(DIR_SRC)/obj
DIR_CFG=./config

EXEC=server
TEST=test_config test_str test_fifo

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

all: $(EXEC) $(TEST)
test: $(TEST)


# Sources dependencies
test_str.c: $(DIR_INC)/str.h $(DIR_SRC_TEST)/str.c
test_config.c: $(DIR_INC)/config.h $(DIR_SRC_TEST)/config.c $(DIR_CFG)/init.cfg
test_fifo.c: $(DIR_SRC_TEST)/fifo.c $(DIR_DEV)/fifo_rd $(DIR_DEV)/fifo_wr $(DIR_INC)/fifo.h

server.c: $(DIR_SRC)/main.c $(DIR_INC)/usage.h

# Objects dependencies
test_str: test_str.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/str.c -o $@

test_config: test_config.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/config.c -o $@

test_fifo: test_fifo.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/fifo.c -o $@

server: server.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/main.c -o $@

# Cleaning
clean: 
	rm -rf $(DIR_OBJ)/*.o *.o
	rm -rf *.exe *.stackdump
	rm -rf $(EXEC)

