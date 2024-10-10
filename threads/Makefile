CC=gcc
CFLAGS=$(flags) -W -Wall -Wextra -Wpedantic
LDFLAGS=-I src -I extern -L build/

all: dirs test/build/bin_test
dirs: src test test/src test/build extern build


src:
	[ -d $@ ] || mkdir $@
test:
	[ -d $@ ] || mkdir $@
test/src:
	[ -d $@ ] || mkdir $@
test/build:
	[ -d $@ ] || mkdir $@
extern:
	[ -d $@ ] || mkdir $@
build:
	[ -d $@ ] || mkdir $@


build/arrayptr.o: src/arrayptr.c src/arrayptr.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/bin_test: test/src/test.c build/arrayptr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


clean:
	rm -f build/*
	rm -f test/build/*

