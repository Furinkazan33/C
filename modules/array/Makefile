CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I src -I test/src -L build -L /usr/lib64


TARGETS_DIRS=build test/build
TARGETS_BUILDS=build/array.o build/libarray.a
TARGETS_BIN_TESTS=test/build/test1
TARGETS_LOG_TESTS=test/build/log1


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


build/array.o: src/array.c src/array.h
	ctags src/array.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/libarray.a: build/array.o
	ar rcsv $@ $^

build/libarray.so: build/array.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@
	cp build/libarray.so /usr/lib64/


test/build/test1: test/src/test1.c build/array.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/log1: ./test/build/test1
	$< > $@


clean:
	rm -f $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
	rmdir $(TARGETS_DIRS) 2> /dev/null; return 0;

