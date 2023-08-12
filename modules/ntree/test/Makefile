CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


DIR_BIN=./bin
DIR_LOG=./log

all: $(DIR_BIN) $(DIR_LOG) tests

$(DIR_BIN):
	mkdir -p $(DIR_BIN)

$(DIR_LOG):
	mkdir -p $(DIR_LOG)

# Objects dependencies
$(DIR_BIN)/ntree_char: ./src/ntree_char.c ../build/libntree.a
	$(CC) $(CFLAGS) -I ./src \
			./src/ntree_char.c \
			-L ../build -lntree \
			-o $@

$(DIR_BIN)/ntree_person: ./src/ntree_person.c
	$(CC) $(CFLAGS) -I ./src \
			./src/ntree_person.c ./src/person.c \
			-L ../build -lntree \
			-o $@


$(DIR_LOG)/ntree_char.log: $(DIR_BIN)/ntree_char
	@echo "########### testing ntree_char #########"
	$(DIR_BIN)/ntree_char > $(DIR_LOG)/ntree_char.log || exit 1
	@echo "########### end testing ntree #####"

$(DIR_LOG)/ntree_person.log: $(DIR_BIN)/ntree_person
	@echo "########### testing ntree_person #########"
	$(DIR_BIN)/ntree_person > $(DIR_LOG)/ntree_person.log|| exit 1
	@echo "########### end testing ntree #####"


tests: $(DIR_LOG)/ntree_char.log $(DIR_LOG)/ntree_person.log


# Cleaning
clean: 
	rm -f $(DIR_BIN)/*
	rm -f $(DIR_LOG)/*
	rm -f *.stackdump

