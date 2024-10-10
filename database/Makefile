CC=gcc
CFLAGS=$(flags) -W -Wall -Wextra -Wpedantic
LDFLAGS=-I src -I src/extern

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


build/arrayptr.o: src/arrayptr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/database.o: src/database.c src/database.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/bin_test: src/test/test.c build/database.o build/arrayptr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/log_test: build/bin_test
	$<

clean:
	rm -f build/*

