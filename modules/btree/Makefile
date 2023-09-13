CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CBUILD)

DIR_CORE=./core
CBUILD=$(DIR_CORE)/build
CSRC=$(DIR_CORE)/src

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/libbtree.a $(CBUILD)/btree.o
tests: $(TBIN)/btree_person $(TBIN)/btree_char
execs: $(TLOG)/btree_char.log $(TLOG)/btree_person.log


#
# Folders creations
#
$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)

#
# Main builds
#
$(CBUILD)/btree.o: $(CSRC)/btree.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(CSRC)/btree.c -o $@

$(CBUILD)/libbtree.a: $(CBUILD)/btree.o
	ar ruv $(CBUILD)/libbtree.a $(CBUILD)/btree.o
	ranlib $(CBUILD)/libbtree.a

#
# Tests compilations
#
$(TBIN)/btree_person: $(TSRC)/btree_person.c $(TSRC)/person.c $(CBUILD)/libbtree.a
	$(CC) $(CFLAGS) $(TSRC)/btree_person.c $(TSRC)/person.c \
			$(LDFLAGS) -lbtree \
			-o $@

$(TBIN)/btree_char: $(TSRC)/btree_char.c $(CBUILD)/libbtree.a
	$(CC) $(CFLAGS) $(TSRC)/btree_char.c \
			$(LDFLAGS) -lbtree \
			-o $@

#
# Tests executions
#
$(TLOG)/btree_char.log: $(TBIN)/btree_char
	$(TBIN)/btree_char > $(TLOG)/btree_char.log

$(TLOG)/btree_person.log: $(TBIN)/btree_person
	$(TBIN)/btree_person > $(TLOG)/btree_person.log

# 
# Cleaning
#
clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	

