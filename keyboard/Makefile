
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
#DEP_O=$(COBJ)/dev.o
#DEP_H=$(CINC)/dev.h
#DEP_A=$(CLIB)/libdev.a

#
# Rules
#
all: dirs builds tests
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/kbd.o $(CBUILD)/libkbd.a
tests: $(TBIN)/test1

debug: CFLAGS+=-DDEBUG
debug: all


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


$(CBUILD)/kbd.o: $(CSRC)/kbd.c $(CSRC)/kbd.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/kbd.c -o $@

$(CBUILD)/libkbd.a: $(CBUILD)/kbd.o
	ar ruv $(CBUILD)/libkbd.a $(CBUILD)/kbd.o $(DEP_O)
	ranlib $(CBUILD)/libkbd.a


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/kbd.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(CBUILD)/kbd.o $(DEP_O) \
		$(TSRC)/test1.c -o $@


##############################
# Cleaning
##############################

clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


