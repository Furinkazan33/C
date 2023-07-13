CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BIN=./bin
DIR_SRC=./src
DIR_SRC_TEST=$(DIR_SRC)/test
DIR_DEV=./dev
DIR_INC=$(DIR_SRC)/include
DIR_OBJ=$(DIR_SRC)/obj
DIR_CFG=./config

MAIN=server
TEST=test_config test_fifo test_tb test_sockets
EXEC=exec_tests

all: $(MAIN) $(TEST) $(EXEC)
main: $(MAIN)
test: $(TEST)
exec: $(EXEC) 

# $@ Le nom de la cible
# $< Le nom de la première dépendance
# $^ La liste des dépendances
# $? La liste des dépendances plus récentes que la cible
# $* Le nom du fichier sans suffixe

# Sources dependencies
test_tb.c: $(DIR_INC)/tb.h $(DIR_SRC_TEST)/tb.c
test_config.c: $(DIR_INC)/config.h $(DIR_SRC_TEST)/config.c $(DIR_CFG)/init.cfg
test_fifo.c: $(DIR_SRC_TEST)/fifo.c $(DIR_DEV)/fifo_rd $(DIR_DEV)/fifo_wr $(DIR_INC)/fifo.h
test_sock_cli.c: $(DIR_INC)/sock_client.h $(DIR_SRC_TEST)/sock_client.c
test_sock_serv.c: $(DIR_INC)/sock_server.h $(DIR_SRC_TEST)/sock_server.c
server.c: $(DIR_SRC)/server.c $(DIR_INC)/usage.h test_tb.c test_config.c test_fifo.c

# Objects dependencies
test_tb: test_tb.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/tb.c -o $(DIR_BIN)/$@

test_config: test_config.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/config.c -o $(DIR_BIN)/$@

test_fifo: test_fifo.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/fifo.c -o $(DIR_BIN)/$@

test_sock_cli: test_sock_cli.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/sock_client.c -o $(DIR_BIN)/$@

test_sock_serv: test_sock_serv.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC_TEST)/sock_server.c -o $(DIR_BIN)/$@

test_sockets: test_sock_cli test_sock_serv

server: server.c
	$(CC) $(CFLAGS) -I$(DIR_INC) $(DIR_SRC)/server.c -o $(DIR_BIN)/$@

exec_tests:
	$(DIR_BIN)/test_tb ressources/ici.txt &> /dev/null && echo "ok" || echo "ko"
	$(DIR_BIN)/test_config &> /dev/null && echo "ok" || echo "ko"

# Cleaning
clean: 
	rm -rf *.stackdump
	rm -rf $(DIR_BIN)/*


