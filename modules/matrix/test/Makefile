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

EXEC=$(LBIN)/main $(LOG)/main

all: $(EXEC)


$(LBIN)/main: ./src/main.c
	$(CC) $(CFLAGS) \
		-I$(INC) -I ../src \
		./src/main.c ../build/matrix.o \
		-o $@


$(LOG)/main: $(LBIN)/main
		$(LBIN)/main > $(LOG)/main \
		|| (rm -f $(LOG)/main; exit 1;)



clean:
	rm -f $(LBIN)/*
	rm -f $(LOG)/*
	rm -f *.stackdump

