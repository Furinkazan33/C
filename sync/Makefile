CC=gcc
CFLAGS=-W -Wall -ansi -std=gnu99 -Wextra -pedantic $(flags)
LDFLAGS=-I src/ -I libs -I ext -L libs -L ext

FOLDERS_TO_CLEAN=ext build test/results test/build


all: test/results/ok_sync


dev_link_dep:
	ln ../libs/src/macros.h ext/
	ln ../libs/src/arrayh.h ext/
	ln ../libs/src/tree.h ext/
	ln ../libs/src/file.h ext/
	ln ../libs/src/filediff.h ext/
	ln ../libs/build/libtree.a ext/
	ln ../libs/build/libfile.a ext/
	ln ../libs/build/libfilediff.a ext/

preprod_copy_dep:
	cp ../libs/src/macros.h ext/
	cp ../libs/src/arrayh.h ext/
	cp ../libs/src/tree.h ext/
	cp ../libs/src/tree.c ext/
	cp ../libs/src/file.h ext/
	cp ../libs/src/file.c ext/
	cp ../libs/src/filediff.h ext/
	cp ../libs/src/filediff.c ext/

prod_make_dep:
	$(CC) $(CFLAGS) $(LDFLAGS) -c ext/tree.c -o ext/tree.o
	ar rv ext/libtree.a ext/tree.o
	ranlib ext/libtree.a
	$(CC) $(CFLAGS) $(LDFLAGS) -c ext/file.c -o ext/file.o
	ar rv ext/libfile.a ext/file.o
	ranlib ext/libfile.a
	$(CC) $(CFLAGS) $(LDFLAGS) -c ext/filediff.c -o ext/filediff.o
	ar rv ext/libfilediff.a ext/filediff.o
	ranlib ext/libfilediff.a
	
	

# object files
build/sync.o: src/sync.c src/sync.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

# tests binaries
test/build/bin_sync: test/src/sync.c build/sync.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -ltree -lfile -lfilediff -o $@

# tests results
test/results/ok_sync: test/build/bin_sync
	rm -f $@
	$< && touch $@

clean:
	$(foreach folder,$(FOLDERS_TO_CLEAN),rm -f $(folder)/*;)
