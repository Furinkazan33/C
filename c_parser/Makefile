
#
# Compiler flags
#
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CINC) -I $(CSRC)
LDFLAGS=-L $(CLIB) -L $(CBUILD)

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
DEP_O=$(COBJ)/list.o  $(COBJ)/fr.o
DEP_H=$(CINC)/list.h $(CINC)/fr.h
DEP_A=$(CLIB)/liblist.a $(CLIB)/libfr.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/lexer.o $(CBUILD)/liblexer.a $(CBUILD)/parser.o $(CBUILD)/libparser.a
tests: $(TBIN)/test1
execs: $(TLOG)/test1 $(TLOG)/test_stdio

debug: CFLAGS += -DDEBUG
debug: all

debug2: CFLAGS += -DDEBUG -DDEBUG2
debug2: all

debug3: CFLAGS += -DDEBUG -DDEBUG2 -DDEBUG3
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
		-c -llist $(CSRC)/lexer.c -o $@

$(CBUILD)/liblexer.a: $(CBUILD)/lexer.o
	ar ruv $(CBUILD)/liblexer.a $(CBUILD)/lexer.o $(DEP_O)
	ranlib $(CBUILD)/liblexer.a


$(CBUILD)/parser.o: $(CSRC)/parser.c $(CSRC)/parser.h ./Makefile $(DEP_O) $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c -llist $(CSRC)/parser.c -o $@

$(CBUILD)/libparser.a: $(CBUILD)/parser.o
	ar ruv $(CBUILD)/libparser.a $(CBUILD)/parser.o $(DEP_O)
	ranlib $(CBUILD)/libparser.a

##############################
# Test builds and executions
##############################

$(TBIN)/test1: $(CBUILD)/lexer.o $(CBUILD)/parser.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) $(DEP_O) \
		$(CBUILD)/lexer.o $(CBUILD)/parser.o \
		$(TSRC)/test1.c -o $@

$(TLOG)/test1: $(TBIN)/test1
	$(TBIN)/test1 $(TRES)/code.c > $(TLOG)/test1

$(TLOG)/test_stdio: $(TBIN)/test1
	$(TBIN)/test1 /usr/include/stdio.h > $(TLOG)/test_stdio


##############################
# Cleaning
##############################

clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


