CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I libs -I src/ -L libs



all: make_libs
all: dirs tags builds test_builds test_results
dirs: src build test test/src test/build test/results
builds: build/XXXX.o
test_builds: test/build/test1
test_results: test/results/ok_1



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



make_libs:
	make -C libs



tags: 
	ctags -R



build/XXXX.o: src/XXXX.c src/XXXX.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ || rm -f $@

test/build/test1: test/src/test1.c build/XXXX.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -larray -o $@  || rm -f $@

test/results/ok_1: ./test/build/test1
	rm -f $@; $< && touch $@



clean:
	make -C libs clean
	rm -Rf build;
	rm -Rf test/results;
	rm -Rf test/build


