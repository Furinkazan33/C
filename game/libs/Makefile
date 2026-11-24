CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I ./ -I ../ext

all: LLLL
LLLL: LLLL.o libLLLL.a



#
# LLLL
#
LLLL.o: LLLL.c LLLL.h macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

libLLLL.a: LLLL.o
	ar rv $@ $<
	ranlib $@



clean:
	rm -Rf *.a *.o


