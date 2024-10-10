CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I src -I test/src -L build


all: dirs test/build/ok_1
dirs: src build test test/src test/build


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


build/XXXX.o: src/XXXX.c src/XXXX.h src/macros.h
	ctags src/XXXX.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test1: test/src/test1.c build/XXXX.o src/macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/build/ok_1: ./test/build/test1
	$< && touch $@ || { rm -f $@; exit 1; }


clean:
	rm -f build/*; rmdir build;
	rm -f test/build/*; rmdir test/build;

