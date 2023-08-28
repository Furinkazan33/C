CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
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
DEP_H=$(CINC)/cJSON.h $(CINC)/fifo.h $(CINC)/fr.h $(CINC)/config.h
#DEP_O=$(COBJ)/cJSON.o $(COBJ)/fifo.o $(COBJ)/fr.o $(COBJ)/config.o


all: dirs builds
dirs: $(CBUILD) $(TBIN) $(TLOG)
builds: $(CBUILD)/server $(CBUILD)/client


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
$(CBUILD)/server: $(CSRC)/server.c $(DEP_A) $(DEP_H)
	$(CC) $(CFLAGS) \
		-I $(CINC) \
		$(CSRC)/server.c $(LDFLAGS) \
		-o $@

$(CBUILD)/client: $(CSRC)/server.c $(DEP_A) $(DEP_H)
	$(CC) $(CFLAGS) \
		-I $(CINC) \
		$(CSRC)/client.c $(LDFLAGS) \
		-o $@



clean: 
	rm -f $(CBUILD)/*


