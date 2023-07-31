CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

INC=/usr/local/include
LIB=/usr/local/lib
SRC=/usr/local/src
BIN=./bin


all: tests


message.c: $(SRC)/message.c $(INC)/message.h

./bin/main_person: $(SRC)/person.c $(INC)/person.h ../build/liblist.a ../src/list.h
	$(CC) $(CFLAGS) -I$(INC) -I ../src \
			./src/test_person.c $(SRC)/person.c \
			-L $(LIB) -lcjson -L ../build -llist \
			-o $@

./bin/main_message: $(SRC)/message.c $(INC)/message.h ../build/liblist.a
	$(CC) $(CFLAGS) -I$(INC) -I ../src \
			./src/test_message.c $(SRC)/message.c \
			-L ../build -llist \
			-o $@

./log/person.log: ./bin/main_person
	@echo "########## Testing -llist #############"
	$(BIN)/main_person > ./log/person.log || exit 1
	@echo "########## end testing -llist #########"

./log/message.log: ./bin/main_message
	@echo "########## Testing -llist #############"
	$(BIN)/main_message > ./log/message.log || exit 1
	@echo "########## end testing -llist #########"

tests: ./log/message.log ./log/person.log

clean: 
	rm -rf $(BIN)/*

