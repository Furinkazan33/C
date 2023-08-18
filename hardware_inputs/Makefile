
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
#DEP_O=$(COBJ)/dev.o
#DEP_H=$(CINC)/dev.h
#DEP_A=$(CLIB)/libdev.a

#
# Rules
#
all: dirs builds tests
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/hwi.o $(CBUILD)/libhwi.a
tests: $(TBIN)/test1




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


$(CBUILD)/hwi.o: $(CSRC)/hwi.c $(CSRC)/hwi.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/hwi.c -o $@

$(CBUILD)/libhwi.a: $(CBUILD)/hwi.o
	ar ruv $(CBUILD)/libhwi.a $(CBUILD)/hwi.o $(DEP_O)
	ranlib $(CBUILD)/libhwi.a


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/hwi.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(CBUILD)/hwi.o $(DEP_O) \
		$(TSRC)/test1.c -o $@


##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/*
	rm -f $(TLOG)/*
	rm -f *.stackdump


