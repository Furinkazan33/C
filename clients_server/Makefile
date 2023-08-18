CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


all: ./bin/server


./bin/server: ./src/server.c
	$(CC) $(CFLAGS) -I ./src -I modules/include \
		./src/server.c \
		-L ./modules/lib -lcjson -lfifo -lfr -lconfig \
		-o $@

clean: 
	rm -f ./bin/*


