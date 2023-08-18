
#
# Compiler flags
#
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-L $(CLIB) -L $(CBUILD) -I $(CINC) -I $(CSRC)

#
# Core folders
#
CSRC=core/src
CBUILD=core/build
COBJ=core/obj
CINC=core/include
CLIB=core/lib

#
# Tests folders
#
TEST=test
TSRC=$(TEST)/src
TBIN=$(TEST)/bin
TLOG=$(TEST)/log
TRES=$(TEST)/ressources

#
# Dependencies
#
DEP_O=$(COBJ)/str.o  $(COBJ)/array.o $(COBJ)/fr.o $(COBJ)/char.o
DEP_H=$(CINC)/str.h $(CINC)/array.h $(CINC)/fr.h $(CINC)/char.h
DEP_A=$(CLIB)/libstr.a $(CLIB)/libarray.a $(CLIB)/libfr.a $(CLIB)/libchar.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/fb.o $(CBUILD)/libfb.a
tests: $(TBIN)/test1 $(TBIN)/test2
execs: test/1 test/2




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


$(CBUILD)/fb.o: $(CSRC)/fb.c ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/fb.c -o $@

$(CBUILD)/libfb.a: $(CBUILD)/fb.o
	ar ruv $(CBUILD)/libfb.a $(CBUILD)/fb.o $(DEP_O)
	ranlib $(CBUILD)/libfb.a


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/fb.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(CBUILD)/fb.o $(DEP_O) \
		$(TSRC)/test1.c -o $@

# compiling test2 with .a
$(TBIN)/test2: $(CBUILD)/libfb.a $(TSRC)/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/test1.c -lfb -o $@

# execution of test 1, result is ok if file '1' is present
$(TEST)/1: $(TBIN)/test1 $(TRES)/file.txt
	$(TBIN)/test1 $(TRES)/file.txt $(TRES)/file.txt.sav1 \
		> $(TLOG)/test1 \
		&& diff $(TRES)/file.txt $(TRES)/file.txt.sav1 \
		&& > $(TEST)/1 \
		|| (rm -f $(TEST)/1; exit 1;)

# execution of test 2, result is ok if file '2' is present
$(TEST)/2: $(TBIN)/test2 $(TRES)/file.txt
	$(TBIN)/test2 $(TRES)/file.txt $(TRES)/file.txt.sav2 \
		> $(TLOG)/test2 \
		&& diff $(TRES)/file.txt $(TRES)/file.txt.sav2 \
		&& > $(TEST)/2 \
		|| (rm -f $(TEST)/2; exit 1;)


##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/*
	rm -f $(TEST)/1 $(TEST)/2
	rm -f $(TLOG)/*
	rm -f $(TRES)/file.txt.sav*
	rm -f *.stackdump


