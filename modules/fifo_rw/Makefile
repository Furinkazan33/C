ifeq ($(ENV),)
	ENV=prod
endif

CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I ext/ -I src/

all: dirs tags test_builds test_results
dirs: ext src build test test/src test/build test/results
test_builds: test/build/test1
test_results: test/results/ok_1

debug: CFLAGS += -DDEBUG
debug: all


ext:
	[ -d $@ ] || mkdir $@

src: 
	[ -d $@ ] || mkdir $@

build: 
	[ -d $@ ] || mkdir $@

test: 
	[ -d $@ ] || mkdir $@

test/src: 
	[ -d $@ ] || mkdir $@

test/build: 
	[ -d $@ ] || mkdir $@

test/results:
	[ -d $@ ] || mkdir $@

# 
# copy external dependencies in dev mode
#
ifeq ($(ENV),dev)

ext/macros.h: /home/mathieu/C/0_template/src/macros.h
	cp $< $@

ext/str.h: /home/mathieu/C/modules/str/src/str.h
	cp $< $@

ext/str.c: /home/mathieu/C/modules/str/src/str.c
	cp $< $@


endif
#
#
#


tags: ext/macros.h ext/str.h
	ctags $^ -lc --declarations --global --defines -o $@

build/str.o: ext/str.c ext/str.h ext/macros.h
	ctags ext/str.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/fifo.o: src/fifo.c src/fifo.h ext/macros.h
	ctags src/fifo.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test1: test/src/test1.c ext/macros.h build/fifo.o build/str.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/results/ok_1: ./test/build/test1
	$< && touch $@ || { rm -f $@; exit 1; }


ifeq ($(ENV),dev)

clean:
	rm -Rf build;
	rm -Rf test/build;
	rm -Rf ext;
	rm -f test/results/ok_*

else

clean:
	rm -Rf build;
	rm -Rf test/build;
	rm -f test/results/ok_*

endif

