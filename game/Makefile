CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I src/ -I libs/ -I ext/ -L libs/ -L ext/

FOLDERS_TO_CLEAN=build test/results test/build


all: test/results/ok_game

libs/%.a:
	make -C libs $(@F)

# object files
build/game.o: src/game.c src/game.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# tests binaries
test/build/bin_game: test/src/game.c build/game.o
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# tests results
test/results/ok_game: test/build/bin_game
	rm -f $@
	$< && touch $@

clean:
	make -C libs clean
	$(foreach folder,$(FOLDERS_TO_CLEAN),rm -f $(folder)/*;)
