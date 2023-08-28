CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=-L ./build/

DIR_SRC=./src
DIR_BUILD=./build
DIR_TEST=./test
DIR_TSRC=./test/src
DIR_TBIN=./test/bin


all: dirs builds tests execs

dirs: $(DIR_BUILD) $(DIR_TBIN)
builds: $(DIR_BUILD)/str.o $(DIR_BUILD)/libstr.a
tests: $(DIR_TBIN)/test1
execs: test1

$(DIR_BUILD):
	test -d $(DIR_BUILD) || mkdir -p $(DIR_BUILD)

$(DIR_TBIN):
	test -d $(DIR_TBIN) || mkdir -p $(DIR_TBIN)

$(DIR_BUILD)/str.o: $(DIR_SRC)/str.c $(DIR_SRC)/str.h
	$(CC) $(CFLAGS) -c $(DIR_SRC)/str.c -o $@

$(DIR_BUILD)/libstr.a: $(DIR_BUILD)/str.o
	ar ruv $(DIR_BUILD)/libstr.a $(DIR_BUILD)/str.o
	ranlib $(DIR_BUILD)/libstr.a


$(DIR_TBIN)/test1: $(DIR_BUILD)/str.o $(DIR_TSRC)/test1.c
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-I $(DIR_SRC) \
		$(DIR_TSRC)/test1.c -lstr -o $@

test1:
	$(DIR_TBIN)/test1


clean:
	rm -f $(DIR_BUILD)/*

