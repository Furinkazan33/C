CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

TARGET=config

DIR_SRC=./src
DIR_INC=/usr/local/include
DIR_LIB=/usr/local/lib

DEP_TB=../0_tb/tb.o

all: deploy
lib:lib$(TARGET).a

$(TARGET).c: $(DIR_SRC)/$(TARGET).c $(DIR_SRC)/$(TARGET).h

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -I$(DIR_SRC) -c $(DIR_SRC)/$(TARGET).c

lib$(TARGET).a: $(TARGET).o $(DEP_TB)
	ar ruv lib$(TARGET).a $(TARGET).o $(DEP_TB)
	ranlib lib$(TARGET).a

deploy: lib$(TARGET).a
	cp lib$(TARGET).a $(DIR_LIB)/
	cp $(DIR_SRC)/$(TARGET).h $(DIR_INC)/	

# Cleaning
clean: 
	rm $(TARGET).o lib$(TARGET).a
	rm -rf *.stackdump

