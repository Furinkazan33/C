CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_BUILD=./build
SOURCES=./src/array.c ./src/array.h 
CTAGS_DEP=$(SOURCES)


all: setup compile deploy tests

setup: dirs
compile: $(DIR_BUILD)/array.o $(DIR_BUILD)/libarray.a 
tests: test


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
# deploying
#
deploy: $(DIR_BUILD)/libarray.a $(DIR_BUILD)/array.o
	cp $(DIR_BUILD)/libarray.a ~/repos/C/fb/core/lib/
	cp $(DIR_BUILD)/libarray.a ~/repos/C/shell/core/lib/
	cp $(DIR_BUILD)/libarray.a ~/repos/C/sheet/core/lib/
	cp $(DIR_BUILD)/libarray.a ~/repos/C/db/core/lib/
	cp $(DIR_BUILD)/array.o ~/repos/C/server/modules/obj/
	cp $(DIR_BUILD)/array.o ~/repos/C/fb/core/obj/
	cp $(DIR_BUILD)/array.o ~/repos/C/sheet/core/obj/
	cp $(DIR_BUILD)/array.o ~/repos/C/db/core/obj/


#
# Tests
#
test: $(DIR_BUILD)/libarray.a
	make -C test


clean:
	rm -f $(DIR_BUILD)/*
	make -C test clean
	rm -f *.stackdump

