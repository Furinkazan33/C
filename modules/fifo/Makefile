CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CBUILD)


CSRC=src
CBUILD=build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/fifo.o
tests: $(TBIN)/main
execs: test/main_ok


$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)

# Main builds
$(CBUILD)/fifo.o: $(CSRC)/fifo.c $(CSRC)/fifo.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/fifo.c -o $@

# Tests builds
$(TBIN)/main: $(TSRC)/main.c $(TSRC)/message.c $(CBUILD)/fifo.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

test/main_ok: $(TBIN)/main
	$< > $(TLOG)/main.log && touch $@ || { rm -f $@; exit 1; }

# Cleaning
clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


