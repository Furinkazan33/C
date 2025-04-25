CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I src -I /home/mathieu/C/ -I test/src


TARGETS_DIRS=build test/build
TARGETS_BUILDS=build/array.o
TARGETS_BIN_TESTS=test/build/test2
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


build/array.o: src/array.c src/array.h src/macros.h
	ctags src/array.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test2: test/src/test2.c build/array.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log2: test/build/test2
	$< > $@


clean:
	rm -f $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
	rmdir $(TARGETS_DIRS) 2> /dev/null; return 0;

