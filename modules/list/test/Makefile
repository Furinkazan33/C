CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log

all: $(DIR_BIN) $(DIR_LOG) tests


$(DIR_BIN):
	test -d $(DIR_BIN) || mkdir -p $(DIR_BIN)

$(DIR_LOG):
	test -d $(DIR_LOG) || mkdir -p $(DIR_LOG)


message.c: ./src/message.c ./src/message.h

$(DIR_BIN)/main_person: ./src/person.c ./src/person.h ../build/liblist.a ../src/list.h
	$(CC) $(CFLAGS) -I ./src -I ../src \
			./src/test_person.c ./src/person.c \
			-L ../build -llist \
			-o $@

$(DIR_BIN)/main_message: ./src/message.c ./src/message.h ../build/liblist.a
	$(CC) $(CFLAGS) -I ./src -I ../src \
			./src/test_message.c ./src/message.c \
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

