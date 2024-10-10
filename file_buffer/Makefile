CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
LDFLAGS=-I external -I src


# Tests folders
TEST=test
TSRC=$(TEST)/src
TBIN=$(TEST)/bin
TLOG=$(TEST)/log
TRES=$(TEST)/ressources


# Rules
all: dirs builds tests execs
dirs: build external $(TBIN) $(TLOG)
builds: build/fb.o
tests: $(TBIN)/load_save
execs: $(TLOG)/load_save $(TLOG)/result


# Directories creation
build:
	test -d $@ || mkdir -p $@

external:
	test -d $@ || mkdir -p $@

$(TBIN):
	test -d $@ || mkdir -p $@

$(TLOG):
	test -d $@ || mkdir -p $@


# Main builds
build/fr.o: external/fr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

build/str.o: external/str.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

build/arrayptr.o: external/arrayptr.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

build/fb.o: src/fb.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


# Test builds and executions
$(TBIN)/load_save: $(TSRC)/load_save.c build/fb.o  build/arrayptr.o build/fr.o build/str.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(TLOG)/load_save: $(TBIN)/load_save $(TRES)/file.txt
	$< $(TRES)/file.txt $(TRES)/file.txt.sav > $@

$(TLOG)/result: $(TLOG)/load_save
	diff $(TRES)/file.txt $(TRES)/file.txt.sav > $@ || exit 1


# Cleaning
clean: 
	if [ -d build ]; then rm -f build/*; rmdir build; fi
	if [ -d $(TBIN) ]; then rm -f $(TBIN)/*; rmdir $(TBIN); fi
	if [ -d $(TLOG) ]; then rm -f $(TLOG)/*; rmdir $(TLOG); fi
	rm -f $(TRES)/file.txt.sav*


