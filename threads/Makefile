CC=gcc
CFLAGS=$(flags) -W -Wall -Wextra -Wpedantic
LDFLAGS=-I src -I extern -L build/

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


build/parray.o: extern/parray.c extern/parray.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/bin_test: test/src/test.c build/parray.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log_test: test/build/bin_test
	$< test/build/ # > $@


clean:
	rm -f build/*
	rm -f test/build/*

