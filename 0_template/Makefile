CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I src/ -I libs/ -I ext/ -L libs/ -L ext/

FOLDERS_TO_CLEAN=build test/results test/build


all: test/results/ok_XXXX

libs/%.a:
	make -C libs $(@F)

# object files
build/XXXX.o: src/XXXX.c src/XXXX.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# tests binaries
test/build/bin_XXXX: test/src/XXXX.c build/XXXX.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# tests results
test/results/ok_XXXX: test/build/bin_XXXX
	rm -f $@
	$< && touch $@

clean:
	make -C libs clean
	$(foreach folder,$(FOLDERS_TO_CLEAN),rm -f $(folder)/*;)
