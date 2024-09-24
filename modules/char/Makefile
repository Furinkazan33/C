CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=
LDFLAGS=-I src -I test/src

all: dirs builds tests execs
dirs: build test/bin test/log
builds: build/char.o
tests: test/bin/test1 test/bin/test2
execs: test/log/test1 test/log/test2


#
# Folers creations
#
build:
	test -d build || mkdir -p build

test/bin:
	test -d test/bin || mkdir -p test/bin

test/log:
	test -d test/log || mkdir -p test/log

# Main build
build/char.o: src/char.c src/char.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Tests
test/bin/test1: test/src/test1.c build/char.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/bin/test2: test/src/test2.c build/char.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# executions
test/log/test1: test/bin/test1
	test/bin/test1 > test/log/test1 || exit 1

test/log/test2: test/bin/test2
	test/bin/test2 > test/log/test2 || exit 1

# Cleaning
clean: 
	if [ -d build ]; then rm -f build/*; rmdir build; fi
	if [ -d test/bin ]; then rm -f test/bin/*; rmdir test/bin; fi
	if [ -d test/log ]; then rm -f test/log/*; rmdir test/log; fi

