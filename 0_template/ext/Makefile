CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I ./

all: libXXXX.a


XXXX.o: XXXX.c XXXX.h macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


lib%.a: %.o
	ar rv $@ $<
	ranlib $@



clean:
	rm -Rf *.a *.o


