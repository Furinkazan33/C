# for debug mode : make -BE flags=-DDEBUG
CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
CFLAGS=-I src -I test/src -I libs
LDFLAGS=



##############################
# targets
##############################
all: make_libs
all: dirs tags builds tests execs
dirs: build test/bin test/logs
builds: build/main
tests:
execs:



##############################
# Directories creation
##############################
build:
	test -d $@ || mkdir -p $@

test/bin:
	test -d $@ || mkdir -p $@

test/logs:
	test -d $@ || mkdir -p $@



##############################
# make_libs
##############################
make_libs:
	make -C libs



##############################
# tags
##############################
tags: 
	ctags -R
	

	
##############################
# Main builds
##############################
# Objects dependencies
build/main: src/object.c src/object.h src/load_save.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

	

##############################
# cleaning
##############################
clean: 
	make -C libs clean
	if [ -d build ]; then rm -f build/*; rmdir build; fi
	if [ -d test/bin ]; then rm -f test/bin/*; rmdir test/bin; fi
	if [ -d test/logs ]; then rm -f test/logs/*; rmdir test/logs; fi

