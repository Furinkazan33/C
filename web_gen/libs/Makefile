CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I ./



all: tags array.o libarray.a




tags: 
	ctags -R



# 
# array
#
array.o: array.c array.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

libarray.a: array.o
	ar rv $@ $<
	ranlib $@



clean:
	rm -Rf *.a *.o tags

