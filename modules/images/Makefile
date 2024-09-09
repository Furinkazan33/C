CC=gcc
CFLAGS=$(flags) -W -Wall -Wextra -Wpedantic
LDFLAGS=-I src -I extern

all: dirs test/build/log_test
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


build/pixels.o: src/pixels.c src/pixels.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/bin_test: test/src/test.c build/pixels.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log_test: test/build/bin_test
	$< test/build/ > $@


clean:
	rm -f build/*
	rm -f test/build/*

