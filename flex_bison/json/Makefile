CC = gcc
OPT = -O3 -std=c11

all: build ./result.txt


build:
	test -d $@ || mkdir $@

build/parser.h build/parser.c: src/parser.y ./Makefile
	bison -d -v -o build/parser.c src/parser.y

build/lexer.h build/lexer.c: src/lexer.l ./Makefile
	flex --header-file=build/lexer.h -o build/lexer.c src/lexer.l 


build/parse_file: build/lexer.c build/parser.c include/types.c include/types.h
	$(CC) $(OPT) -I ./include $^ -o $@


./result.txt: build/parse_file ./ressources/cells.json
	./build/parse_file ./ressources/cells.json > ./result.txt && cat ./result.txt



clean:
	rm -f *~
	rm -f build/*
	rm -f ./result.txt


