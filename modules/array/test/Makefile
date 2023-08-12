CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


SRC=/usr/local/src
LIB=/usr/local/lib
INC=/usr/local/include
RES=/usr/local/ressources

DIR_BIN=./bin
DIR_LOG=./log


all= dirs execs tests

dirs=$(DIR_LOG) $(DIR_BIN)
execs=$(DIR_BIN)/main $(DIR_BIN)/map_reduce
tests=$(DIR_LOG)/main $(DIR_LOG)/map_reduce

$(DIR_BIN):
	mkdir $(DIR_BIN)

$(DIR_LOG):
	mkdir $(DIR_LOG)


$(DIR_BIN)/main: ./src/main.c ../build/array.o $(SRC)/person.c $(LIB)/libcjson.a
	$(CC) $(CFLAGS) \
		-I$(INC) -I ../src \
		$(SRC)/person.c ./src/main.c ../build/array.o \
		-L $(LIB) -lcjson \
		-o $@


$(DIR_LOG)/main: $(DIR_BIN)/main $(RES)/persons.json
		$(DIR_BIN)/main $(RES)/persons.json > $(DIR_LOG)/main \
		|| (rm -f $(DIR_LOG)/main; exit 1;)


$(DIR_BIN)/map_reduce: ./src/map_reduce.c ../build/array.o
		$(CC) $(CFLAGS) \
		-I$(INC) -I ../src \
		./src/map_reduce.c ../build/array.o \
		-o $@

$(DIR_LOG)/map_reduce: $(DIR_BIN)/map_reduce
		$(DIR_BIN)/map_reduce > $(DIR_LOG)/map_reduce \
		|| (rm -f $(DIR_LOG)/map_reduce; exit 1;)



clean:
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

