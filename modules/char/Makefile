CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=
LDFLAGS=-I $(CSRC) -I $(TSRC)

DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/char.o $(CBUILD)/libchar.a
tests: $(TBIN)/test1 $(TBIN)/test2
execs: $(TLOG)/test1 $(TLOG)/test2


#
# Folers creations
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
$(CBUILD)/char.o: $(CSRC)/char.c $(CSRC)/char.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(CSRC)/char.c -o $@

$(CBUILD)/libchar.a: $(CBUILD)/char.o
	ar ruv $(CBUILD)/libchar.a $(CBUILD)/char.o
	ranlib $(CBUILD)/libchar.a

#
# Tests compilations
#
$(TBIN)/test1: $(TSRC)/test1.c $(CBUILD)/libchar.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(CBUILD)/char.o $(TSRC)/test1.c -o $@

$(TBIN)/test2: $(TSRC)/test2.c $(CBUILD)/libchar.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(CBUILD)/char.o $(TSRC)/test2.c -o $@


#
# Tests executions
#
$(TLOG)/test1: $(TBIN)/test1
	$(TBIN)/test1 > $(TLOG)/test1 || exit 1

$(TLOG)/test2: $(TBIN)/test2
	$(TBIN)/test2 > $(TLOG)/test2 || exit 1


#
# Cleaning
#
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi

