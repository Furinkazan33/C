
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
DEP_O=$(COBJ)/array.o $(COBJ)/str.o
DEP_H=$(CINC)/array.h $(CINC)/str.h
DEP_A=$(CLIB)/libarray.a $(CLIB)/libstr.a

#
# Rules
#
all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/db.o $(CBUILD)/libdb.a
tests:  $(TBIN)/create $(TBIN)/load_save
execs: $(TLOG)/create $(TLOG)/load_save $(TLOG)/result

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


$(CBUILD)/db.o: $(CSRC)/db.c $(CSRC)/db.h ./Makefile $(DEP_O) $(DEP_H)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c -lstr -larray $(CSRC)/db.c -o $@

$(CBUILD)/libdb.a: $(CBUILD)/db.o
	ar ruv $(CBUILD)/libdb.a $(CBUILD)/db.o $(DEP_O)
	ranlib $(CBUILD)/libdb.a


##############################
# Test builds and executions
##############################

# compiling create test with .o
$(TBIN)/create: $(CBUILD)/db.o $(TSRC)/create.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(DEP_O) \
		$(CBUILD)/db.o \
		$(TSRC)/create.c -o $@

# execution of create test, result is ok if file 'create' is present
$(TLOG)/create: $(TBIN)/create
	$(TBIN)/create $(TRES)/create.db > $(TLOG)/create

# compiling load_save test with .o
$(TBIN)/load_save: $(CBUILD)/db.o $(TSRC)/load_save.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(DEP_O) \
		$(CBUILD)/db.o \
		$(TSRC)/load_save.c -o $@

# execution of load_save test, result is ok if file 'load_save' is present
$(TLOG)/load_save: $(TBIN)/load_save
	$(TBIN)/load_save $(TRES)/create.db $(TRES)/load_save.db > $(TLOG)/load_save

$(TLOG)/result: $(TLOG)/create $(TLOG)/load_save
	@diff $(TRES)/create.db $(TRES)/load_save.db > $(TLOG)/result || exit 1


##############################
# Cleaning
##############################

clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


