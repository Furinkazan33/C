CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
LDFLAGS=-I src -L build

# Rules
all: dirs builds tests
dirs: src build test test/src test/bin test/log
builds: build/kbd.o build/libkbd.a
tests: test/bin/test1

# Directories creation
src:
	test -d $@ || mkdir -p $@
build:
	test -d $@ || mkdir -p $@
test:
	test -d $@ || mkdir -p $@
test/src:
	test -d $@ || mkdir -p $@
test/bin:
	test -d $@ || mkdir -p $@
test/log:
	test -d $@ || mkdir -p $@


# Main builds
build/kbd.o: src/kbd.c src/kbd.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/libkbd.a: build/kbd.o
	ar ruv build/libkbd.a build/kbd.o
	ranlib build/libkbd.a


# Test builds and executions
test/bin/test1: build/kbd.o test/src/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


# Cleaning
clean: 
	if [ -d build ]; then rm -f build/*; rmdir build; fi
	if [ -d test/bin ]; then rm -f test/bin/*; rmdir test/bin; fi
	if [ -d test/log ]; then rm -f test/log/*; rmdir test/log; fi


