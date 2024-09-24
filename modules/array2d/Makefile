CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I src -I test/src -L build


TARGETS_DIRS=build test/build
TARGETS_BUILDS=build/array2d.o build/libarray2d.a
TARGETS_BIN_TESTS=test/build/test1
TARGETS_LOG_TESTS=test/build/log1


all: dirs $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
dirs: src test test/src extern $(TARGETS_DIRS)


debug: CFLAGS += -DDEBUG
debug: all

src: 
	[ -d $@ ] || mkdir $@
test: 
	[ -d $@ ] || mkdir $@
test/src: 
	[ -d $@ ] || mkdir $@
extern: 
	[ -d $@ ] || mkdir $@
build: 
	[ -d $@ ] || mkdir $@
test/build: 
	[ -d $@ ] || mkdir $@


build/array2d.o: src/array2d.c src/array2d.h
	ctags -R
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/libarray2d.a: build/array2d.o
	ar rcsv $@ $^

test/build/test1: test/src/test1.c build/array2d.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log1: ./test/build/test1
	$< > $@


clean:
	rm -f $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS) tags
	rmdir $(TARGETS_DIRS) 2> /dev/null; return 0;

