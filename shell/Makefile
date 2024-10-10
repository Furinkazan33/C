# Compiler flags
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
CFLAGS=$(flags)
LDFLAGS=-L $(CLIB) -L $(CBUILD) -I $(CINC) -I $(CSRC)

# Core folders
CSRC=./core/src
CBUILD=./core/build
COBJ=./core/obj
CINC=./core/include
CLIB=./core/lib

# Tests folders
TEST=./test
TSRC=$(TEST)/src
TBIN=$(TEST)/bin
TLOG=$(TEST)/log
TRES=$(TEST)/ressources

# Rules
all: dirs builds tests
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/shell.o
tests: $(TBIN)/test1


# Directories creation
$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)


# Main builds
$(CBUILD)/shell.o: $(CSRC)/shell.c $(CSRC)/shell.h $(DEP_H) $(DEP_A)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(CSRC)/shell.c $(LDLIBS) -o $@


# Test builds and executions
$(TBIN)/test1: $(CBUILD)/shell.o $(TSRC)/test1.c $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) $(TSRC)/test1.c $(CBUILD)/shell.o -o $@


# Cleaning
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


