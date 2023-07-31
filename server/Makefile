CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src

all: server

socket:
	cp ./src/include/socket.h $(INC)/

server: socket
	$(CC) $(CFLAGS) -I $(INC) -I ./src/include ./src/server.c $(SRC)/config.c $(SRC)/frd.c \
		-L $(LIB) -ltb -lcjson \
		-o ./bin/$@

clean: 
	rm -f *.stackdump
	rm -f ./bin/server


