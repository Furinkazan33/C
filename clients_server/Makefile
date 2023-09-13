CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CINC) -I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CLIB) -lcjson -lfifo -lfr -lconfig

CORE=./core
CBUILD=./core/build
CLIB=./core/lib
COBJ=./core/obj
CINC=./core/include
CSRC=./core/src

TEST=./test
TSRC=./test/src
TBIN=./test/bin
TLOG=./test/log


DEP_A=$(CLIB)/libcjson.a $(CLIB)/libfifo.a $(CLIB)/libfr.a $(CLIB)/libconfig.a
DEP_H=$(CINC)/cJSON.h $(CINC)/fifo.h $(CINC)/fr.h $(CINC)/config.h $(CSRC)/socket.h
#DEP_O=$(COBJ)/cJSON.o $(COBJ)/fifo.o $(COBJ)/fr.o $(COBJ)/config.o


all: dirs builds tests execs
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/socket.o $(CBUILD)/server.o $(CBUILD)/client.o
tests: $(TBIN)/server $(TBIN)/client
execs:


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
$(CBUILD)/socket.o: $(CSRC)/socket.c $(CSRC)/socket.h
	$(CC) $(CFLAGS) \
		-c $(CSRC)/socket.c $(LDFLAGS) \
		-o $@

$(CBUILD)/server.o: $(CSRC)/server.c $(CSRC)/server.h $(CBUILD)/socket.o $(DEP_A) $(DEP_H)
	$(CC) $(CFLAGS) \
		-c $(CSRC)/server.c $(LDFLAGS) \
		-o $@

$(CBUILD)/client.o: $(CSRC)/client.c $(CSRC)/client.h $(CBUILD)/socket.o $(DEP_A) $(DEP_H)
	$(CC) $(CFLAGS) \
		-c $(CSRC)/client.c $(LDFLAGS) \
		-o $@

$(TBIN)/server: $(CBUILD)/server.o $(TSRC)/server.c
	$(CC) $(CFLAGS) \
		$(TSRC)/server.c $(CBUILD)/socket.o $(CBUILD)/server.o $(LDFLAGS) \
		-o $@

$(TBIN)/client: $(CBUILD)/client.o $(TSRC)/client.c
	$(CC) $(CFLAGS) \
		$(TSRC)/client.c $(CBUILD)/socket.o $(CBUILD)/client.o $(LDFLAGS) \
		-o $@




clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi


