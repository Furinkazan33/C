CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=


DIR_CORE=./core
CBUILD=$(DIR_CORE)/build
CSRC=$(DIR_CORE)/src

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/mem.o
tests: $(TBIN)/main
execs: $(TLOG)/main

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
$(CBUILD)/mem.o: $(CSRC)/mem.c $(CSRC)/mem.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/mem.c -o $@


#
# Tests
#
$(TBIN)/main: $(TSRC)/main.c $(CBUILD)/mem.o
	$(CC) $(CFLAGS) $(TSRC)/main.c $(CBUILD)/mem.o -o $@

$(TLOG)/main: $(TBIN)/main
	$(TBIN)/main > $(TLOG)/main || exit 1


#
# Cleaning
#
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi

