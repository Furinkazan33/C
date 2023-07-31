CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_SRC=/usr/local/src
DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib
DIR_BIN=./bin


all: tests


# Objects dependencies
$(DIR_BIN)/ntree_char: ./src/ntree_char.c ../build/libntree.a
	$(CC) $(CFLAGS) -I$(DIR_INC) \
			./src/ntree_char.c \
			-L ../build -lntree \
			-o $@

$(DIR_BIN)/ntree_person: ./src/ntree_person.c
	$(CC) $(CFLAGS) -I$(DIR_INC) \
			./src/ntree_person.c $(DIR_SRC)/person.c \
			-L ../build -lntree -L $(DIR_LIB) -lcjson \
			-o $@


./log/ntree_char.log: $(DIR_BIN)/ntree_char
	@echo "########### testing ntree_char #########"
	$(DIR_BIN)/ntree_char > ./log/ntree_char.log || exit 1
	@echo "########### end testing ntree #####"

./log/ntree_person.log: $(DIR_BIN)/ntree_person
	@echo "########### testing ntree_person #########"
	$(DIR_BIN)/ntree_person > ./log/ntree_person.log|| exit 1
	@echo "########### end testing ntree #####"


tests: ./log/ntree_char.log ./log/ntree_person.log


# Cleaning
clean: 
	rm -rf $(DIR_BIN)/*
	rm -rf *.stackdump

