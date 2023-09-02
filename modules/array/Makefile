CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-I $(CSRC) -I $(TSRC)

DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TLOG) $(TBIN)
builds: $(CBUILD)/array.o $(CBUILD)/libarray.a
tests: $(TBIN)/main $(TBIN)/map_reduce $(TBIN)/sort $(TBIN)/find_all
execs: $(TLOG)/main $(TLOG)/map_reduce $(TLOG)/sort $(TLOG)/find_all

debug: CFLAGS += -DDEBUG
debug: all

#
# Setup
#
$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)

#
# Compile
#
$(CBUILD)/array.o: $(CSRC)/array.c $(CSRC)/array.h ./Makefile
	ctags -R
	$(CC) $(CFLAGS) -c $(CSRC)/array.c -o $@

$(CBUILD)/libarray.a: $(CBUILD)/array.o
	ar ruv $(CBUILD)/libarray.a $(CBUILD)/array.o
	ranlib $(CBUILD)/libarray.a

#
# Tests compilations
#
$(TBIN)/main: $(TSRC)/main.c $(CBUILD)/array.o $(TSRC)/person.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/person.c $(TSRC)/main.c $(CBUILD)/array.o \
		-o $@

$(TBIN)/map_reduce: $(TSRC)/map_reduce.c $(CBUILD)/array.o
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/map_reduce.c $(CBUILD)/array.o \
		-o $@

$(TBIN)/sort: $(TSRC)/sort.c $(CBUILD)/array.o $(TSRC)/person.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/person.c $(TSRC)/sort.c $(CBUILD)/array.o \
		-o $@

$(TBIN)/find_all: $(TSRC)/find_all.c $(CBUILD)/array.o $(TSRC)/person.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/person.c $(TSRC)/find_all.c $(CBUILD)/array.o \
		-o $@

# 
# Tests executions
#
$(TLOG)/main: $(TBIN)/main
	$(TBIN)/main > $(TLOG)/main || (rm -f $(TLOG)/main; exit 1;)

$(TLOG)/map_reduce: $(TBIN)/map_reduce
	$(TBIN)/map_reduce > $(TLOG)/map_reduce || (rm -f $(TLOG)/map_reduce; exit 1;)

$(TLOG)/sort: $(TBIN)/sort
	$(TBIN)/sort > $(TLOG)/sort || (rm -f $(TLOG)/sort; exit 1;)

$(TLOG)/find_all: $(TBIN)/find_all
	$(TBIN)/find_all > $(TLOG)/find_all || (rm -f $(TLOG)/find_all; exit 1;)


#
# Cleaning
#
clean:
	rm -f $(CBUILD)/*
	rmdir $(CBUILD)
	rm -f $(TBIN)/*
	rmdir $(TBIN)
	rm -f $(TLOG)/*
	rmdir $(TLOG)

