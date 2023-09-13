CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=

DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/fr.o $(CBUILD)/libfr.a


$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)



$(CBUILD)/fr.o: $(CSRC)/fr.c $(CSRC)/fr.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/fr.c -o $@

$(CBUILD)/libfr.a: $(CBUILD)/fr.o
	ar ruv $(CBUILD)/libfr.a $(CBUILD)/fr.o
	ranlib $(CBUILD)/libfr.a



clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi

