CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=

CBUILD=build
CSRC=src

DIR_TEST=test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/cipher.o
tests: $(TBIN)/encode
execs: $(TLOG)/encode


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
# Main build
#
$(CBUILD)/cipher.o: $(CSRC)/cipher.c $(CSRC)/cipher.h
	$(CC) $(CFLAGS) -c $(CSRC)/cipher.c -o $@

#
# Tests builds
#
$(TBIN)/encode: $(CBUILD)/cipher.o
	$(CC) $(CFLAGS) $(TSRC)/encode.c $(CBUILD)/cipher.o -o $@

#
# Tests executions
#
$(TLOG)/encode: $(TBIN)/encode
	$(TBIN)/encode 10 > $(TLOG)/encode
	$(TBIN)/encode  2 >> $(TLOG)/encode
	$(TBIN)/encode 16 >> $(TLOG)/encode

#
# Cleaning
#
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


