CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src

DIR_BIN=./bin
DIR_LOG=./log

all: $(DIR_BIN) $(DIR_LOG) tests


$(DIR_BIN):
	mkdir $(DIR_BIN)

$(DIR_LOG):
	mkdir $(DIR_LOG)


message.c: $(SRC)/message.c $(INC)/message.h

$(DIR_BIN)/main_person: $(SRC)/person.c $(INC)/person.h ../build/liblist.a ../src/list.h
	$(CC) $(CFLAGS) -I$(INC) -I ../src \
			./src/test_person.c $(SRC)/person.c \
			-L $(LIB) -lcjson -L ../build -llist \
			-o $@

$(DIR_BIN)/main_message: $(SRC)/message.c $(INC)/message.h ../build/liblist.a
	$(CC) $(CFLAGS) -I$(INC) -I ../src \
			./src/test_message.c $(SRC)/message.c \
			-L ../build -llist \
			-o $@

$(DIR_LOG)/person.log: $(DIR_BIN)/main_person
	@echo "########## Testing -llist #############"
	$(DIR_BIN)/main_person > $(DIR_LOG)/person.log || exit 1
	@echo "########## end testing -llist #########"

$(DIR_LOG)/message.log: $(DIR_BIN)/main_message
	@echo "########## Testing -llist #############"
	$(DIR_BIN)/main_message > $(DIR_LOG)/message.log || exit 1
	@echo "########## end testing -llist #########"

tests: $(DIR_LOG)/message.log $(DIR_LOG)/person.log

clean: 
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*

