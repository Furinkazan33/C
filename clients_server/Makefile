CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I $(CINC) -I $(CSRC) -I $(TSRC)
LDFLAGS=-L $(CLIB) -L /usr/lib64

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

$(CBUILD)/config.o: $(CSRC)/config.c $(CSRC)/config.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(CBUILD)/fr.o: $(CSRC)/fr.c $(CSRC)/fr.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(CBUILD)/server.o: $(CSRC)/server.c $(CSRC)/server.h $(CBUILD)/socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(CBUILD)/client.o: $(CSRC)/client.c $(CSRC)/client.h $(CBUILD)/socket.o
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

$(TBIN)/server: $(TSRC)/server.c $(CBUILD)/server.o $(CBUILD)/socket.o $(CBUILD)/config.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ -lcjson

$(TBIN)/client: $(TSRC)/client.c $(CBUILD)/client.o $(CBUILD)/socket.o $(CBUILD)/fr.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@




clean: 
	if [ -d $(CBUILD) ]; then rm -f $(CBUILD)/*; rmdir $(CBUILD); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi


