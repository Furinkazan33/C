CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=btree

DIR_SRC=./src
DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib

all: deploy
lib:lib$(TARGET).a

$(TARGET).c: $(DIR_SRC)/$(TARGET).c $(DIR_SRC)/$(TARGET).h

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -I$(DIR_SRC) -c $(DIR_SRC)/$(TARGET).c -o $@

lib$(TARGET).a: $(TARGET).o
	ar ruv lib$(TARGET).a $(TARGET).o
	ranlib lib$(TARGET).a

deploy: lib$(TARGET).a
	cp lib$(TARGET).a $(DIR_LIB)/
	cp $(DIR_SRC)/$(TARGET).h $(DIR_INC)/	

# Cleaning
clean: 
	rm $(TARGET).o lib$(TARGET).a
	rm -rf *.stackdump

