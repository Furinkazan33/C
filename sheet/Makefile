
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
DEP_O=$(COBJ)/array.o $(COBJ)/fr.o
DEP_H=$(CINC)/array.h $(CINC)/fr.h
DEP_A=$(CLIB)/libarray.a $(CLIB)/libfr.a

#
# Rules
#
all: dirs builds tests #execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/book.o $(CBUILD)/libbook.a
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
# TODO: toremove (modules dependencies)
##############################

$(CLIB)/libfr.a: ../modules/file_reader/build/libfr.a
	cp ../modules/file_reader/build/libfr.a $(CLIB)
$(CLIB)/libarray.a: ../modules/array/build/libarray.a
	cp ../modules/array/build/libarray.a $(CLIB)

$(COBJ)/fr.o: ../modules/file_reader/build/fr.o
	cp ../modules/file_reader/build/fr.o $(COBJ)
$(COBJ)/array.o: ../modules/array/build/array.o 
	cp ../modules/array/build/array.o $(COBJ)

$(CINC)/fr.h: ../modules/file_reader/src/fr.h 
	cp ../modules/file_reader/src/fr.h $(CINC)
$(CINC)/array.h: ../modules/array/src/array.h 
	cp ../modules/array/src/array.h $(CINC)


##############################
# Main builds
##############################

$(CBUILD)/book.o: $(CSRC)/book.c ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/book.c -o $@

$(CBUILD)/libbook.a: $(CBUILD)/book.o
	ar ruv $(CBUILD)/libbook.a $(CBUILD)/book.o $(DEP_O)
	ranlib $(CBUILD)/libbook.a


##############################
# Test builds and executions
##############################

# compiling test1 with .a
$(TBIN)/test1: $(CBUILD)/libbook.a $(TSRC)/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/test1.c -lbook -o $@

# execution of test1, result is ok if file '1' is present
#$(TEST)/1: $(TBIN)/test1 $(TRES)/file.txt
#	$(TBIN)/test1 $(TRES)/file.txt $(TRES)/file.txt.sav1 \
#		> $(TLOG)/test1 \
#		&& diff $(TRES)/file.txt $(TRES)/file.txt.sav1 \
#		&& > $(TEST)/1 \
#		|| (rm -f $(TEST)/2; exit 1;)


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


