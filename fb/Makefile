
#
# Compiler flags
#
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-L $(CLIB) -L $(CBUILD) -I $(CINC) -I $(CSRC)

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
DEP_O=$(COBJ)/str.o  $(COBJ)/array.o $(COBJ)/fr.o $(COBJ)/char.o
DEP_H=$(CINC)/str.h $(CINC)/array.h $(CINC)/fr.h $(CINC)/char.h
DEP_A=$(CLIB)/libstr.a $(CLIB)/libarray.a $(CLIB)/libfr.a $(CLIB)/libchar.a

#
# Rules
#
all: build test
build: ./core/build/fb.o ./core/build/libfb.a
test/bin: test/bin/test1 test/bin/test2
test: test/1 test/2


##############################
# TODO: toremove (modules dependencies)
##############################

$(CLIB)/libfr.a: ../modules/file_reader/build/libfr.a
	cp ../modules/file_reader/build/libfr.a $(CLIB)
$(CLIB)/libstr.a: ../modules/str/build/libstr.a
	cp ../modules/str/build/libstr.a $(CLIB)
$(CLIB)/libarray.a: ../modules/array/build/libarray.a
	cp ../modules/array/build/libarray.a $(CLIB)
$(CLIB)/libchar.a: ../modules/char/build/libchar.a
	cp ../modules/char/build/libchar.a $(CLIB)

$(COBJ)/fr.o: ../modules/file_reader/build/fr.o
	cp ../modules/file_reader/build/fr.o $(COBJ)
$(COBJ)/str.o: ../modules/str/build/str.o 
	cp ../modules/str/build/str.o $(COBJ)
$(COBJ)/array.o: ../modules/array/build/array.o 
	cp ../modules/array/build/array.o $(COBJ)
$(COBJ)/char.o: ../modules/char/build/char.o 
	cp ../modules/char/build/char.o $(COBJ)

$(CINC)/fr.h: ../modules/file_reader/src/fr.h 
	cp ../modules/file_reader/src/fr.h $(CINC)
$(CINC)/str.h: ../modules/str/src/str.h 
	cp ../modules/str/src/str.h $(CINC)
$(CINC)/array.h: ../modules/array/src/array.h 
	cp ../modules/array/src/array.h $(CINC)
$(CINC)/char.h: ../modules/char/src/char.h 
	cp ../modules/char/src/char.h $(CINC)


##############################
# Main builds
##############################


$(CBUILD)/fb.o: $(CSRC)/fb.c $(DEP_H) $(DEP_A) ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-c $(CSRC)/fb.c -larray -lstr -lfr -lchar -o $@

$(CBUILD)/libfb.a: $(CBUILD)/fb.o
	ar ruv $(CBUILD)/libfb.a $(CBUILD)/fb.o
	ranlib $(CBUILD)/libfb.a


##############################
# Test builds and executions
##############################

# compiling test1 with .o
$(TBIN)/test1: $(CBUILD)/fb.o $(TSRC)/test1.c $(DEP_O)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(DEP_O) $(CBUILD)/fb.o \
		$(TSRC)/test1.c -o $@

# compiling test2 with .a
$(TBIN)/test2: $(CBUILD)/libfb.a $(TSRC)/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		$(TSRC)/test1.c -lfb -o $@

# execution of test 1, result is ok if file '1' is present
./test/1: $(TBIN)/test1 $(TRES)/file.txt
	$(TBIN)/test1 $(TRES)/file.txt $(TRES)/file.txt.sav1 \
		> $(TLOG)/test1 \
		&& diff $(TRES)/file.txt $(TRES)/file.txt.sav1 \
		&& > ./test/1 \
		|| (rm -f ./test/1; exit 1;)

# execution of test 2, result is ok if file '2' is present
./test/2: $(TBIN)/test2 $(TRES)/file.txt
	$(TBIN)/test2 $(TRES)/file.txt $(TRES)/file.txt.sav2 \
		> $(TLOG)/test2 \
		&& diff $(TRES)/file.txt $(TRES)/file.txt.sav2 \
		&& > ./test/2 \
		|| (rm -f ./test/2; exit 1;)


##############################
# Cleaning
##############################

clean: 
	rm -f $(CBUILD)/*
	rm -f $(TBIN)/* ./test/1 ./test/2 $(TLOG)/*
	rm -f $(TRES)/file.txt.sav*
	rm -f *.stackdump


