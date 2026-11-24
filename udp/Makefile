CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I src/ -I libs/ -L libs/

FOLDERS_TO_CLEAN=build test/results test/build


all: test/build/bin_server test/build/bin_client

libs/%.a:
	make -C libs $(@F)

# object files
#build/%.o: src/%.c #src/%.h
#	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# tests binaries
test/build/bin_%: test/src/%.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# tests results
#test/results/ok_%: test/build/bin_%
#	rm -f $@
#	$< && touch $@

clean:
	make -C libs clean
	$(foreach folder,$(FOLDERS_TO_CLEAN),rm -f $(folder)/*;)
