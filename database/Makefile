
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
DEP_O=$(COBJ)/array.o
DEP_H=$(CINC)/array.h
DEP_A=$(CLIB)/libarray.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/db.o $(CBUILD)/libdb.a
tests: $(TBIN)/test1
execs: test/1




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


$(CBUILD)/db.o: $(CSRC)/db.c $(CSRC)/db.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/db.c -o $@

$(CBUILD)/libdb.a: $(CBUILD)/db.o
	ar ruv $(CBUILD)/libdb.a $(CBUILD)/db.o $(DEP_O)
	ranlib $(CBUILD)/libdb.a


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/db.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(CBUILD)/db.o $(DEP_O) \
		$(TSRC)/test1.c -o $@

# compiling test2 with .a
$(TBIN)/test2: $(CBUILD)/libdb.a $(TSRC)/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/test1.c -ldb -o $@

# execution of test 1, result is ok if file '1' is present
$(TEST)/1: $(TBIN)/test1
	$(TBIN)/test1 \
		> $(TLOG)/test1 \
		&& > $(TEST)/1 || (rm -f $(TEST)/1; exit 1;)


##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/*
	rm -f $(TEST)/1
	rm -f $(TLOG)/*
	rm -f *.stackdump


