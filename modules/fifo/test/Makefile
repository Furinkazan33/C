CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log


all: dirs builds test

dirs: $(DIR_BIN) $(DIR_LOG)
builds: $(DIR_BIN)/main
test: $(DIR_LOG)/main.log


$(DIR_BIN):
	mkdir -p $(DIR_BIN)

$(DIR_LOG):
	mkdir -p $(DIR_LOG)


$(DIR_BIN)/main: ./src/main.c ./src/message.c
	$(CC) $(CFLAGS) ./src/main.c \
			-I../src/ -I ./src \
			-L../build/ -lfifo \
			./src/message.c \
			-o $@

$(DIR_LOG)/main.log: $(DIR_BIN)/main
	@echo "############ test fifo #############"
	$(DIR_BIN)/main > $(DIR_LOG)/main.log || exit 1
	@echo "############ test end ##############"



clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

