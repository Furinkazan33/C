CC=gcc
CFLAGS=$(flags) -W -Wall -Wextra -Wpedantic
LDFLAGS=-I src -I src/extern -I /usr/include

all: dirs execs
dirs: src src/extern src/test build
execs: build/log_test


src:
	[ -d $@ ] || mkdir $@
src/extern:
	[ -d $@ ] || mkdir $@
src/test:
	[ -d $@ ] || mkdir $@
build:
	[ -d $@ ] || mkdir $@


build/pix2d.o: src/pix2d.c src/pix2d.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@ -lm



build/bin_test: src/test/test.c build/pix2d.o build/array2d.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@



build/log_test: build/bin_test
	$<



clean:
	rm -f build/*

