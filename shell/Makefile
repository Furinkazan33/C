
#
# Compiler flags
#
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-L $(CLIB) -L $(CBUILD) -I $(CINC) -I $(CSRC)
LDLIBS=

#
# Core folders
#
CSRC=./core/src
CBUILD=./core/build
COBJ=./core/obj
CINC=./core/include
CLIB=./core/lib

#
# Tests folders
#
TEST=./test
TSRC=$(TEST)/src
TBIN=$(TEST)/bin
TLOG=$(TEST)/log
TRES=$(TEST)/ressources

#
# Dependencies
#
#DEP_O=$(COBJ)/str.o  $(COBJ)/array.o $(COBJ)/fr.o $(COBJ)/char.o
#DEP_H=$(CINC)/str.h $(CINC)/array.h $(CINC)/fr.h $(CINC)/char.h
#DEP_A=$(CLIB)/libstr.a $(CLIB)/libarray.a $(CLIB)/libfr.a $(CLIB)/libchar.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/shell.o
tests: $(TBIN)/test1
execs: $(TEST)/1


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
$(CBUILD)/shell.o: $(CSRC)/shell.c $(CSRC)/shell.h $(DEP_H) $(DEP_A)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/shell.c $(LDLIBS) -o $@


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/shell.o $(TSRC)/test1.c $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/test1.c $(CBUILD)/shell.o -o $@

./test/1: $(TBIN)/test1
	$(TBIN)/test1 

##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/*
	rm -f $(TEST)/1
	rm -f $(TLOG)/*
	rm -f *.stackdump


