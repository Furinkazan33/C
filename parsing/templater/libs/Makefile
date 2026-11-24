CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I ./

all: tags



tags: 
	ctags -R



# 
# XXXX
#
XXXX.o: XXXX.c XXXX.h macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

libXXXX.a: XXXX.o
	ar rv $@  $<
	ranlib $@



clean:
	rm -Rf *.a *.o tags

