CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

LSRC=./src
LBUILD=./build
LMODS=./modules

DEP_MODS_O=$(LMODS)/obj/*.o 
DEP_MODS_I=$(LMODS)/include
DEP_MODS_H=$(LMODS)/include/*.h


all: build test

build: ./build/fb.o ./build/libfb.a

test: ./test/bin/test_o ./test/bin/test_o.log ./test/bin/test_lib ./test/bin/test_lib.log


./build/fb.o: ./src/fb.c $(DEP_MODS_O) $(DEP_MODS_H)
	$(CC) $(CFLAGS) -I ./modules/include \
			-c ./src/fb.c \
			-o ./build/fb.o

./test/bin/test_o: ./build/fb.o $(DEP_MODS_H) ./test/src/test1.c
	$(CC) $(CFLAGS) -I ./modules/include -I ./src \
			$(DEP_MODS_O) ./build/fb.o ./test/src/test1.c \
			-o ./test/bin/test_o

./test/bin/test_o.log: ./test/bin/test_o ./test/ressources/file.txt
	./test/bin/test_o ./test/ressources/file.txt \
			> ./test/bin/test_o.log \
			&& diff ./test/bin/test_o.log ./test/ressources/file.txt \
			|| exit 1



./build/libfb.a: $(DEP_MODS_O) ./build/fb.o
	ar ruv ./build/libfb.a ./build/fb.o $(DEP_MODS_O)
	ranlib ./build/libfb.a


./test/bin/test_lib: ./build/libfb.a ./test/src/test1.c $(DEP_MODS_H)
	$(CC) $(CFLAGS) \
		-I ./modules/include -I ./src \
		./test/src/test1.c \
		-L ./build -lfb \
		-o ./test/bin/test_lib

./test/bin/test_lib.log: ./test/bin/test_lib ./test/ressources/file.txt
	./test/bin/test_lib ./test/ressources/file.txt \
			> ./test/bin/test_lib.log \
			&& diff ./test/bin/test_lib.log ./test/ressources/file.txt \
			|| exit 1

clean: 
	rm -f *.stackdump


