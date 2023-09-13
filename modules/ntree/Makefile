CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CBUILD)

DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/ntree.o $(CBUILD)/libntree.a
tests: $(TBIN)/ntree_char $(TBIN)/ntree_person
execs: $(TLOG)/ntree_char.log $(TLOG)/ntree_person.log


#
# Folders creations
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
$(CBUILD)/ntree.o: $(CSRC)/ntree.c $(CSRC)/ntree.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/ntree.c -o $@

$(CBUILD)/libntree.a: $(CBUILD)/ntree.o
	ar ruv $(CBUILD)/libntree.a $(CBUILD)/ntree.o
	ranlib $(CBUILD)/libntree.a

#
# Tests
#
$(TBIN)/ntree_char: $(TSRC)/ntree_char.c $(CBUILD)/libntree.a
	$(CC) $(CFLAGS) $(TSRC)/ntree_char.c \
			$(LDFLAGS) -lntree \
			-o $@

$(TBIN)/ntree_person: $(TSRC)/ntree_person.c $(CBUILD)/libntree.a
	$(CC) $(CFLAGS) $(TSRC)/ntree_person.c $(TSRC)/person.c \
			$(LDFLAGS) -lntree \
			-o $@

$(TLOG)/ntree_char.log: $(TBIN)/ntree_char
	@echo "########### testing ntree_char #########"
	$(TBIN)/ntree_char > $(TLOG)/ntree_char.log || exit 1
	@echo "########### end testing ntree #####"

$(TLOG)/ntree_person.log: $(TBIN)/ntree_person
	@echo "########### testing ntree_person #########"
	$(TBIN)/ntree_person > $(TLOG)/ntree_person.log|| exit 1
	@echo "########### end testing ntree #####"


#
# Cleaning
#
clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


