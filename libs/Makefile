CC=gcc
CFLAGS=-W -Wall -Wextra -pedantic -Wpadded $(flags)
LDFLAGS=-I ./src -I ./tests -L ./build


TESTS=arrayh cipher fifo file filediff grid list_message list_person str ht db db2 tree rex #parser_calcul #kbd


# tests results
all: $(addprefix tests/ok_,$(TESTS))

clean:
	rm -f build/*.o build/*.a tests/bin_* tests/ok_* *.dump


.PHONY: new_lib
new_lib:
	[ -z "$(NAME)" ] && NAME=new_lib
	touch src/$(NAME).c
	touch src/$(NAME).h
	echo "#include <.h>\n#include <.h>\n#include \"$(NAME).h\"\n\nint main() {\n}\n" > tests/test_$(NAME).c
	


# 
# creating libs
#
build/%.o: src/%.c src/%.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/lib%.a: build/%.o
	ar rv $@ $<
	ranlib $@




# 
# creating tests binaries
#
arrayh_bin=tests/bin_arrayh
arrayh_srcs=tests/test_arrayh.c
arrayh_deps=src/arrayh.h
arrayh_libs=
arrayh_result=tests/ok_arrayh

cipher_bin=tests/bin_cipher
cipher_srcs=tests/test_cipher.c
cipher_deps=build/libcipher.a
cipher_libs=-lcipher
cipher_result=tests/ok_cipher

fifo_bin=tests/bin_fifo
fifo_srcs=tests/test_fifo.c tests/message.c
fifo_deps=build/libfifo.a
fifo_libs=-lfifo
fifo_result=tests/ok_fifo

file_bin=tests/bin_file
file_srcs=tests/test_file.c
file_deps=build/libfile.a
file_libs=-lfile
file_result=tests/ok_file

filediff_bin=tests/bin_filediff
filediff_srcs=tests/test_filediff.c
filediff_deps=build/libfilediff.a
filediff_libs=-lfilediff
filediff_result=tests/ok_filediff

grid_bin=tests/bin_grid
grid_srcs=tests/test_grid.c
grid_deps=build/libgrid.a
grid_libs=-lgrid
grid_result=tests/ok_grid

kbd_bin=tests/bin_kbd
kbd_srcs=tests/test_kbd.c
kbd_deps=build/libkbd.a
kbd_libs=-lkbd
kbd_result=tests/ok_kbd

list_message_bin=tests/bin_list_message
list_message_srcs=tests/test_list_message.c tests/message.c
list_message_deps=build/liblist.a
list_message_libs=-llist
list_message_result=tests/ok_list_message

list_person_bin=tests/bin_list_person
list_person_srcs=tests/test_list_person.c tests/person.c
list_person_deps=build/liblist.a
list_person_libs=-llist
list_person_result=tests/ok_list_person

rex_bin=tests/bin_rex
rex_srcs=tests/test_rex.c
rex_deps=build/librex.a
rex_libs=-lrex
rex_result=tests/ok_rex

str_bin=tests/bin_str
str_srcs=tests/test_str.c
str_deps=build/libstr.a
str_libs=-lstr
str_result=tests/ok_str

ht_bin=tests/bin_ht
ht_srcs=tests/test_ht.c
ht_deps=build/libht.a
ht_libs=-lht
ht_result=tests/ok_ht

db_bin=tests/bin_db
db_srcs=tests/test_db.c
db_deps=build/libdb.a build/libht.a
db_libs=-ldb -lht
db_result=tests/ok_db

db2_bin=tests/bin_db2
db2_srcs=tests/test_db2.c
db2_deps=build/libdb.a build/libht.a
db2_libs=-ldb -lht
db2_result=tests/ok_db2

tree_bin=tests/bin_tree
tree_srcs=tests/test_tree.c
tree_deps=build/libtree.a
tree_libs=-ltree
tree_result=tests/ok_tree

parser_calcul_bin=tests/bin_parser_calcul
parser_calcul_srcs=tests/test_parser_calcul.c
parser_calcul_deps=build/libparser_calcul.a
parser_calcul_libs=-lparser_calcul
parser_calcul_result=tests/ok_parser_calcul




define CREATE_test_bin =
$(1): $(2) $(3)
	$(CC) $(CFLAGS) $(LDFLAGS) $(2) $(4) -o $$@
endef


$(foreach test,$(TESTS),$(eval $(call CREATE_test_bin,\
$($(test)_bin),\
$($(test)_srcs),\
$($(test)_deps),\
$($(test)_libs),\
)))



# 
# creating tests results
#
define CREATE_test_result =
$(1): $(2)
	rm -f $$@
	$$<
	touch $$@
endef

$(foreach test,$(TESTS),$(eval $(call CREATE_test_result,\
$($(test)_result),\
$($(test)_bin),\
)))


