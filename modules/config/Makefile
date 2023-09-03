CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
CFLAGS=-I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CBUILD)


DIR_CORE=./core
CSRC=$(DIR_CORE)/src
CLIB=$(DIR_CORE)/lib
CINC=$(DIR_CORE)/include
CBUILD=$(DIR_CORE)/build

DIR_TEST=./test
TSRC=$(DIR_TEST)/src
TBIN=$(DIR_TEST)/bin
TLOG=$(DIR_TEST)/log
TRES=$(DIR_TEST)/ressources


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/config.o $(CBUILD)/libconfig.a
tests: $(TBIN)/main
execs: $(TLOG)/main.log


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
$(CBUILD)/config.o: $(CSRC)/config.c $(CSRC)/config.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/config.c \
			$(LDFLAGS) -lcjson \
			-o $@

$(CBUILD)/libconfig.a: $(CBUILD)/config.o
	ar ruv $(CBUILD)/libconfig.a $(CBUILD)/config.o
	ranlib $(CBUILD)/libconfig.a


#
# Tests compilations
#
$(TBIN)/main: $(CBUILD)/libconfig.a
	$(CC) $(CFLAGS) $(TSRC)/main.c \
			$(LDFLAGS) -lconfig -lcjson \
			-o $@

#
# Tests executions
#
$(TLOG)/main.log: $(TBIN)/main
	@echo "########## Testing config #################"
	rm -f $(TRES)/config.json2
	$(TBIN)/main $(TRES)/config.json $(TRES)/config.json2 > $(TLOG)/main.log || exit 1
	@echo "########## end testing config #############"


#
# Cleaning
#
clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	rm -f $(TRES)/config.json2


