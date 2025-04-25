CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=

DIR_CORE=./core
CBUILD=$(DIR_CORE)/build
CSRC=$(DIR_CORE)/src
CINC=$(DIR_CORE)/include
COBJ=$(DIR_CORE)/obj

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/main
tests:
execs:


##############################
# Directories creation
##############################
$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)


##############################
# Main builds
##############################



# Objects dependencies
$(CBUILD)/main: $(CSRC)/object.c $(CSRC)/object.h $(CSRC)/load_save.c
	$(CC) $(CFLAGS) $^ -o $@
	

# Cleaning
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi

