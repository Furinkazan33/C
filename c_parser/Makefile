
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
DEP_O=$(COBJ)/array.o $(COBJ)/str.o $(COBJ)/fr.o
DEP_H=$(CINC)/array.h $(CINC)/str.h $(CINC)/fr.h
DEP_A=$(CLIB)/libarray.a $(CLIB)/libstr.a $(CLIB)/libfr.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/lexer.o $(CBUILD)/liblexer.a
tests: $(TBIN)/test1
execs: $(TLOG)/test1

debug1: CFLAGS += -DDEBUG1
debug1: all

debug2: CFLAGS += -DDEBUG1 -DDEBUG2
debug2: all

debug3: CFLAGS += -DDEBUG1 -DDEBUG2 -DDEBUG3
debug3: all


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


$(CBUILD)/lexer.o: $(CSRC)/lexer.c $(CSRC)/lexer.h ./Makefile $(DEP_O) $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c -lstr -larray $(CSRC)/lexer.c -o $@

$(CBUILD)/liblexer.a: $(CBUILD)/lexer.o
	ar ruv $(CBUILD)/liblexer.a $(CBUILD)/lexer.o $(DEP_O)
	ranlib $(CBUILD)/liblexer.a


##############################
# Test builds and executions
##############################

# compiling test1 test with .o
$(TBIN)/test1: $(CBUILD)/lexer.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(DEP_O) \
		$(CBUILD)/lexer.o \
		$(TSRC)/test1.c -o $@

# execution of test1 test, result is ok if file 'test1' is present
$(TLOG)/test1: $(TBIN)/test1
	$(TBIN)/test1 $(TRES)/code.c > $(TLOG)/test1


##############################
# Cleaning
##############################

clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


