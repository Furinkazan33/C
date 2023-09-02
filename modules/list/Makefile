CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-I $(CSRC) -I $(TSRC)

#
# No external dependency for this module (ie no include/, obj/, lib/)
#


DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/list.o $(CBUILD)/liblist.a
tests: $(TBIN)/main_person $(TBIN)/main_message
execs: $(TLOG)/message.log $(TLOG)/person.log

#
# Folders creation
#
$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)

#
# Main builds
#
$(CBUILD)/list.o: $(CSRC)/list.c $(CSRC)/list.h ./Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -c $(CSRC)/list.c -o $@

$(CBUILD)/liblist.a: $(CBUILD)/list.o
	ar ruv $(CBUILD)/liblist.a $(CBUILD)/list.o
	ranlib $(CBUILD)/liblist.a

#
# Tests
#
$(TBIN)/main_person: $(TSRC)/person.c $(TSRC)/person.h $(CBUILD)/liblist.a $(CSRC)/list.h
	$(CC) $(CFLAGS) $(LDFLAGS) \
			$(TSRC)/test_person.c $(TSRC)/person.c \
			-L $(CBUILD) -llist \
			-o $@

$(TBIN)/main_message: $(TSRC)/message.c $(TSRC)/message.h $(CBUILD)/liblist.a
	$(CC) $(CFLAGS) $(LDFLAGS) \
			$(TSRC)/test_message.c $(TSRC)/message.c \
			-L $(CBUILD) -llist \
			-o $@

$(TLOG)/person.log: $(TBIN)/main_person
	@echo "########## Testing -llist #############"
	$(TBIN)/main_person > $(TLOG)/person.log || exit 1
	@echo "########## end testing -llist #########"

$(TLOG)/message.log: $(TBIN)/main_message
	@echo "########## Testing -llist #############"
	$(TBIN)/main_message > $(TLOG)/message.log || exit 1
	@echo "########## end testing -llist #########"

# 
# Cleaning
#
clean:
	rm -f $(CBUILD)/*
	rmdir $(CBUILD)
	rm -f $(TBIN)/*
	rmdir $(TBIN)
	rm -f $(TLOG)/*
	rmdir $(TLOG)

