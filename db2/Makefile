CC=gcc -W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L $(flags)
LDFLAGS=-L build -I src


all: dirs builds tests execs
dirs: src src/extern src/test build ressources
builds: build/db.o build/libdb.a
tests:  build/create build/load_save
execs: build/create build/load_save build/result

debug1: CFLAGS += -DDEBUG1
debug1: all

debug2: CFLAGS += -DDEBUG1 -DDEBUG2
debug2: all

debug3: CFLAGS += -DDEBUG1 -DDEBUG2 -DDEBUG3
debug3: all


# Directories creation
src:
	test -d $@ || mkdir -p $@
src/extern:
	test -d $@ || mkdir -p $@
src/test:
	test -d $@ || mkdir -p $@
build:
	test -d $@ || mkdir -p $@
ressources:
	test -d $@ || mkdir -p $@

build/parray.o: src/parray.c src/parray.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/str.o: src/str.c src/str.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/db.o: src/db.c src/db.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

build/libdb.a: build/db.o
	ar ruv build/libdb.a build/db.o
	ranlib build/libdb.a


# Test builds and executions
build/create: build/db.o src/test/create.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/log_create: build/create
	$< ressources/create.db > $@

build/load_save: build/db.o src/test/load_save.c build/parray.o build/str.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/log_load_save: build/load_save
	$< ressources/create.db ressources/load_save.db > $@

build/result: build/create build/load_save
	@diff ressources/create.db ressources/load_save.db > build/result || exit 1


clean: 
	if [ -d build ]; then rm -f build/*; rmdir build; fi


