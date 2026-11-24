CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I ./

all: libht.a libdb.a



ht.o: ht.c ht.h macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

db.o: db.c db.h macros.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


lib%.a: %.o
	ar rv $@ $<
	ranlib $@



clean:
	rm -Rf *.a *.o


