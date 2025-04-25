CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I src -I /home/mathieu/C/ -I test/src


TARGETS_DIRS=build test/build
TARGETS_BUILDS=build/arrayptr.o
TARGETS_BIN_TESTS=test/build/test1 test/build/test2
TARGETS_LOG_TESTS=test/build/log1 test/build/log2


all: dirs $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
dirs: src test test/src $(TARGETS_DIRS)


debug: CFLAGS += -DDEBUG
debug: all

src: 
	[ -d $@ ] || mkdir $@
test: 
	[ -d $@ ] || mkdir $@
test/src: 
	[ -d $@ ] || mkdir $@
build: 
	[ -d $@ ] || mkdir $@
test/build: 
	[ -d $@ ] || mkdir $@


build/arrayptr.o: src/arrayptr.c src/arrayptr.h src/macros.h
	ctags src/arrayptr.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test1: test/src/test1.c build/arrayptr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/test2: test/src/test2.c build/arrayptr.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log1: test/build/test1
	$< > $@

test/build/log2: test/build/test2
	$< > $@


clean:
	rm -f $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
	rmdir $(TARGETS_DIRS) 2> /dev/null; return 0;

