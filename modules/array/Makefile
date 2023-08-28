CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BUILD=./build
SOURCES=./src/array.c ./src/array.h 
CTAGS_DEP=$(SOURCES)


all: test


#
# Setup
#
dirs:
	test -d $(DIR_BUILD) || mkdir -p $(DIR_BUILD)

#
# Compile
#
$(DIR_BUILD)/array.o: $(SOURCES)
	ctags -R
	$(CC) $(CFLAGS) -c ./src/array.c -o $@

$(DIR_BUILD)/libarray.a: $(DIR_BUILD)/array.o
	ar ruv $(DIR_BUILD)/libarray.a $(DIR_BUILD)/array.o
	ranlib $(DIR_BUILD)/libarray.a

#
# Tests
#
test: $(DIR_BUILD)/libarray.a
	make -C test



clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean
	rm -f *.stackdump

