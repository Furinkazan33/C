CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log


all: dirs execs tests

dirs: $(DIR_LOG) $(DIR_BIN)
execs: $(DIR_BIN)/main $(DIR_BIN)/map_reduce $(DIR_BIN)/sort
tests: $(DIR_LOG)/main $(DIR_LOG)/map_reduce $(DIR_LOG)/sort

$(DIR_BIN):
	test -d $(DIR_BIN) || mkdir -p $(DIR_BIN)

$(DIR_LOG):
	test -d $(DIR_LOG) || mkdir -p $(DIR_LOG)


$(DIR_BIN)/main: ./src/main.c ../build/array.o ./src/person.c
	$(CC) $(CFLAGS) \
		-I./src -I ../src \
		./src/person.c ./src/main.c ../build/array.o \
		-o $@


$(DIR_LOG)/main: $(DIR_BIN)/main
		$(DIR_BIN)/main > $(DIR_LOG)/main \
		|| (rm -f $(DIR_LOG)/main; exit 1;)


$(DIR_BIN)/map_reduce: ./src/map_reduce.c ../build/array.o
		$(CC) $(CFLAGS) \
		-I ../src \
		./src/map_reduce.c ../build/array.o \
		-o $@

$(DIR_LOG)/map_reduce: $(DIR_BIN)/map_reduce
		$(DIR_BIN)/map_reduce > $(DIR_LOG)/map_reduce \
		|| (rm -f $(DIR_LOG)/map_reduce; exit 1;)


$(DIR_BIN)/sort: ./src/sort.c ../build/array.o ./src/person.c
	$(CC) $(CFLAGS) \
		-I./src -I ../src \
		./src/person.c ./src/sort.c ../build/array.o \
		-o $@


$(DIR_LOG)/sort: $(DIR_BIN)/sort
		$(DIR_BIN)/sort > $(DIR_LOG)/sort \
		|| (rm -f $(DIR_LOG)/sort; exit 1;)


clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

