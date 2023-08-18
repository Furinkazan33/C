
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
all: builds
builds: $(CBUILD)/shell


##############################
# Main builds
##############################
$(CBUILD)/shell: $(CSRC)/shell.c $(DEP_H) $(DEP_A)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(CSRC)/shell.c $(LDLIBS) -o $@


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/shell.o $(TSRC)/test1.c $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(DEP_O) $(CBUILD)/fb.o \
		$(TSRC)/test1.c $(LDLIBS) -o $@

# execution of test 1, result is ok if file '1' is present
./test/1: $(TBIN)/test1
	$(TBIN)/test1 \
		> $(TLOG)/test1 \
		&& > ./test/1 \
		|| (rm -f ./test/1; exit 1;)

##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/*
	rm -f $(TEST)/1
	rm -f $(TLOG)/*
	rm -f *.stackdump


