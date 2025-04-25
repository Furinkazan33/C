# dependencies : 
# - cjson lib

CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
LDFLAGS=-I src -I $(TSRC) -L $(CBUILD)


CBUILD=build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log
TRES=$(DIR_TEST)/ressources


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/json.o
tests: $(TBIN)/main
execs: test/main_ok


# Folders creations
$(CBUILD):
	test -d $@ || mkdir -p $@

$(TBIN):
	test -d $@ || mkdir -p $@

$(TLOG):
	test -d $@ || mkdir -p $@


# Main builds
$(CBUILD)/json.o: src/json.c src/json.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -lcjson -o $@


# Tests compilations
$(TBIN)/main: $(TSRC)/main.c $(CBUILD)/json.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -lcjson -o $@

# Tests executions
$(TLOG)/main.log: $(TBIN)/main
	$< $(TRES)/config.json $(TRES)/config.json2 > $@

test/main_ok: $(TLOG)/main.log
	diff $(TRES)/config.json $(TRES)/config.json2 && touch $@ || { rm -f $@; exit 1; }


clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	rm -f $(TRES)/config.json2


