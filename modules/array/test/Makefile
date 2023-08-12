CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LBIN=./bin
LLOG=../log

SRC=/usr/local/src
LIB=/usr/local/lib
INC=/usr/local/include
RES=/usr/local/ressources

LOG=./log

EXEC=$(LBIN)/main $(LOG)/main $(LOG)/map_reduce

all: $(EXEC)


$(LBIN)/main: ./src/main.c ../build/array.o $(SRC)/person.c $(LIB)/libcjson.a
	$(CC) $(CFLAGS) \
		-I$(INC) -I ../src \
		$(SRC)/person.c ./src/main.c ../build/array.o \
		-L $(LIB) -lcjson \
		-o $@


$(LOG)/main: $(LBIN)/main $(RES)/persons.json
		$(LBIN)/main $(RES)/persons.json > $(LOG)/main \
		|| (rm -f $(LOG)/main; exit 1;)


$(LBIN)/map_reduce: ./src/map_reduce.c ../build/array.o
		$(CC) $(CFLAGS) \
		-I$(INC) -I ../src \
		./src/map_reduce.c ../build/array.o \
		-o $@

$(LOG)/map_reduce: $(LBIN)/map_reduce
		$(LBIN)/map_reduce > $(LOG)/map_reduce \
		|| (rm -f $(LOG)/map_reduce; exit 1;)



clean:
	rm -f $(LBIN)/*
	rm -f $(LOG)/*
	rm -f *.stackdump

