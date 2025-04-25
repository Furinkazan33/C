ifeq ($(ENV),)
	ENV=prod
endif

CC=gcc -W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I ext/ -I src/

all: dirs tags builds_ext builds test_builds test_results
dirs: ext src build test test/src test/build test/results
builds_ext: ext/arrayptr.o
builds: build/lexer.o build/parser.o
test_builds: test/build/test_parser
test_results: test/results/parser_ok

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

ext/arrayptr.h: /home/mathieu/C/modules/arrayptr/src/arrayptr.h
	cp $< $@

ext/arrayptr.c: /home/mathieu/C/modules/arrayptr/src/arrayptr.c
	cp $< $@

endif
#
#
#

ext/arrayptr.o: ext/arrayptr.c ext/arrayptr.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

tags: ext/macros.h ext/arrayptr.h
	ctags $^ -lc --declarations --global --defines -o $@

#
# lexer
#
build/lexer.o: src/lexer.c src/lexer.h ext/macros.h
	ctags src/lexer.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test_lexer: test/src/test_lexer.c build/lexer.o ext/arrayptr.o ext/macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/results/lexer_ok: ./test/build/test_lexer ./test/ressources/file.c
	$^ $@


#
# parser
#
build/parser.o: src/parser.c src/parser.h ext/macros.h
	ctags src/parser.h -lc --declarations --global --defines -o tags
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test/build/test_parser: test/src/test_parser.c build/parser.o build/lexer.o ext/arrayptr.o ext/macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/results/parser_ok: ./test/build/test_parser ./test/ressources/file.c
	$^ $@



ifeq ($(ENV),dev)

clean:
	rm -f build/*; rmdir build;
	rm -f test/build/*; rmdir test/build;
	rm -f ext/*.*; rmdir ext;

else

clean:
	rm -f build/*; rmdir build;
	rm -f test/build/*; rmdir test/build;

endif

