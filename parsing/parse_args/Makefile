# for debug mode : make -BE flags=-DDEBUG
CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I libs -I src/ -L libs



#
# targets
#
all: make_libs
all: dirs tags test/build/test
dirs: src build test test/src test/build test/results



#
# folders creations
#
src: 
	[ -d $@ ] || mkdir $@

build: 
	[ -d $@ ] || mkdir $@

test: 
	[ -d $@ ] || mkdir $@

test/src: 
	[ -d $@ ] || mkdir $@

test/build: 
	[ -d $@ ] || mkdir $@

test/results:
	[ -d $@ ] || mkdir $@



#
# make_libs
#
make_libs:
	make -C libs



#
# tags
#
tags: 
	ctags -R



#
# .o files
#
build/parse.o: src/parse.c src/parse.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@



#
# Tests lexers
#
# c as text
test/build/test: test/src/test.c build/parse.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -larray -o $@



#
# clean
#
clean:
	make -C libs clean
	rm -f build/*; rmdir build;
	rm -f test/build/*; rmdir test/build;
	rm -f test/results/*;

