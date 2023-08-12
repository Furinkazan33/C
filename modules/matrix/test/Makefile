CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BIN=./bin
DIR_LOG=./log


all: dirs builds
dirs: $(DIR_BIN) $(DIR_LOG)
builds: $(DIR_BIN)/main $(DIR_LOG)/main


$(DIR_BIN):
	mkdir $(DIR_BIN)

$(DIR_LOG):
	mkdir $(DIR_LOG)



$(DIR_BIN)/main: ./src/main.c
	$(CC) $(CFLAGS) \
		-I ../src \
		./src/main.c ../build/matrix.o \
		-o $@


$(DIR_LOG)/main: $(DIR_BIN)/main
		$(DIR_BIN)/main > $(DIR_LOG)/main \
		|| (rm -f $(DIR_LOG)/main; exit 1;)



clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

