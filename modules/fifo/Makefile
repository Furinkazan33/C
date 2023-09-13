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
builds: $(CBUILD)/fifo.o $(CBUILD)/libfifo.a
tests: $(TBIN)/main
execs: $(TLOG)/main.log


$(CBUILD):
	test -d $(CBUILD) || mkdir -p $(CBUILD)

$(TBIN):
	test -d $(TBIN) || mkdir -p $(TBIN)

$(TLOG):
	test -d $(TLOG) || mkdir -p $(TLOG)

#
# Main builds
# 
$(CBUILD)/fifo.o: $(CSRC)/fifo.c $(CSRC)/fifo.h ./Makefile
	$(CC) $(CFLAGS) -c $(CSRC)/fifo.c -o $@

$(CBUILD)/libfifo.a: $(CBUILD)/fifo.o
	ar ruv $(CBUILD)/libfifo.a $(CBUILD)/fifo.o
	ranlib $(CBUILD)/libfifo.a

#
# Tests builds
#
$(TBIN)/main: $(TSRC)/main.c $(TSRC)/message.c
	$(CC) $(CFLAGS) $(TSRC)/main.c \
			$(LDFLAGS) -lfifo \
			$(TSRC)/message.c \
			-o $@

$(TLOG)/main.log: $(TBIN)/main
	@echo "############ test fifo #############"
	$(TBIN)/main > $(TLOG)/main.log || exit 1
	@echo "############ test end ##############"

#
# Cleaning
#
clean:
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi


