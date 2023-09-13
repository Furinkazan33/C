CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CBUILD)


DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/str.o $(CBUILD)/libstr.a
tests: $(TBIN)/test1 $(TBIN)/escape_normalize
execs: $(TLOG)/test1 $(TLOG)/escape_normalize

debug: CFLAGS += -DDEBUG
debug: all


$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)


$(CBUILD)/str.o: $(CSRC)/str.c $(CSRC)/str.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(CSRC)/str.c -o $@

$(CBUILD)/libstr.a: $(CBUILD)/str.o
	ar ruv $(CBUILD)/libstr.a $(CBUILD)/str.o
	ranlib $(CBUILD)/libstr.a


$(TBIN)/test1: $(CBUILD)/str.o $(TSRC)/test1.c
	$(CC) $(CFLAGS) $(TSRC)/test1.c $(LDFLAGS) -lstr -o $@

$(TLOG)/test1: $(TBIN)/test1
	$(TBIN)/test1 > $(TLOG)/test1

$(TBIN)/escape_normalize: $(CBUILD)/str.o $(TSRC)/escape_normalize.c
	$(CC) $(CFLAGS) $(TSRC)/escape_normalize.c $(LDFLAGS) -lstr -o $@

$(TLOG)/escape_normalize: $(TBIN)/escape_normalize
	$(TBIN)/escape_normalize > $(TLOG)/escape_normalize


clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


