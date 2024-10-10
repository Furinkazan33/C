# Compiler flags
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
LDFLAGS=-I extern -I src -L build

all: dirs builds tests execs
dirs: build test/bin test/log
builds: build/lexer.o build/parser.o
tests: test/bin/test1
execs: test/log/test1 test/log/test_stdio

debug: CFLAGS += -DDEBUG
debug: all

debug2: CFLAGS += -DDEBUG -DDEBUG2
debug2: all

debug3: CFLAGS += -DDEBUG -DDEBUG2 -DDEBUG3
debug3: all

# Directories creation
build:
	test -d $@ || mkdir -p $@

test/bin:
	test -d $@ || mkdir -p $@

test/log:
	test -d $@ || mkdir -p $@

# Main builds
build/list.o: extern/list.c extern/list.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/lexer.o: src/lexer.c src/lexer.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/parser.o: src/parser.c src/parser.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# Tests
test/bin/test1: build/lexer.o build/parser.o test/src/test1.c build/list.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/log/test1: test/bin/test1
	test/bin/test1 test/ressources/code.c > test/log/test1

test/log/test_stdio: test/bin/test1
	test/bin/test1 /usr/include/stdio.h > test/log/test_stdio

# Cleaning
clean: 
	rm -f test/src/*~
	rm -f src/*~
	if [ -d build ]; then rm -f build/*; rmdir build; fi
	if [ -d test/bin ]; then rm -f test/bin/*; rmdir test/bin; fi
	if [ -d test/log ]; then rm -f test/log/*; rmdir test/log; fi


