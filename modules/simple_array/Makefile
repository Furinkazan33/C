CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I src -I test/src -L build -L /usr/lib64


TARGETS_DIRS=build test/build
TARGETS_BUILDS=build/sarray.o build/libsarray.a build/libsarray.so
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


build/sarray.o: src/sarray.c src/sarray.h
	ctags -R
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/libsarray.a: build/sarray.o
	ar rcsv $@ $^

build/libsarray.so: build/sarray.o
	$(CC) $(CFLAGS) $(LDFLAGS) -shared $^ -o $@
	cp build/libsarray.so /usr/lib64/


test/build/test1: test/src/test1.c 
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@ -lsarray

test/build/log1: test/build/test1
	$< > $@


clean:
	rm -f $(TARGETS_BUILDS) $(TARGETS_BIN_TESTS) $(TARGETS_LOG_TESTS)
	rmdir $(TARGETS_DIRS) 2> /dev/null; return 0;

