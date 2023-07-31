CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

SRC=/usr/local/src
INC=/usr/local/include
OBJ=/usr/local/obj

all: exec


./bin/main: ./src/main.c $(SRC)/message.c
	$(CC) $(CFLAGS) ./src/main.c \
			-I../src/ -I $(INC) \
			-L../build/ -lfifo \
			$(SRC)/message.c \
			-o $@

./log/main.log: ./bin/main
	@echo "############ test fifo #############"
	./bin/main > ./log/main.log || exit 1
	@echo "############ test end ##############"


exec: ./log/main.log

clean:
	rm -f ./bin/* ./log/*
	rm -f *.stackdump

