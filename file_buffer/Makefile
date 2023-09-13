
#
# Compiler flags
#
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
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
DEP_O=$(COBJ)/str.o  $(COBJ)/array.o $(COBJ)/fr.o
DEP_H=$(CINC)/str.h $(CINC)/array.h $(CINC)/fr.h
DEP_A=$(CLIB)/libstr.a $(CLIB)/libarray.a $(CLIB)/libfr.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/fb.o $(CBUILD)/libfb.a
tests: $(TBIN)/load_save
execs: $(TLOG)/load_save $(TLOG)/result




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
$(TBIN)/load_save: $(CBUILD)/libfb.a $(TSRC)/load_save.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/load_save.c -lfb -o $@

$(TLOG)/load_save: $(TBIN)/load_save $(TRES)/file.txt
	$(TBIN)/load_save $(TRES)/file.txt $(TRES)/file.txt.sav \
		> $(TLOG)/load_save

$(TLOG)/result: $(TLOG)/load_save
	@diff $(TRES)/file.txt $(TRES)/file.txt.sav > $(TLOG)/result || exit 1


##############################
# Cleaning
##############################

clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	rm -f $(TRES)/file.txt.sav*


