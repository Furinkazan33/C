ifeq ($(ENV),)
	ENV=prod
endif

CC=gcc -W -Wall -Wextra -pedantic $(flags)
CFLAGS=
LDFLAGS=-I ext/ -I src/

all: dirs tags builds
dirs: ext build src
builds: build/test_fr build/test_hashmap build/test_we

debug: CFLAGS += -DDEBUG
debug: all


ext:
	test -d $@ || mkdir $@

src:
	test -d $@ || mkdir $@

build:
	test -d $@ || mkdir $@


# 
# copy external dependencies in dev mode
#
ifeq ($(ENV),dev)

TEMPLATE_SRC=/home/mathieu/C/0_template/src
MACRO_H=$(TEMPLATE_SRC)/macros.h

ext/macros.h: $(MACRO_H)
	cp $< $@

endif
#
#
#

build/test_fr: src/fr.c src/fr.h test_fr.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/test_hashmap: test_hashmap.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

build/test_we: test_wordexp.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


ifeq ($(ENV),dev)

clean:
	rm -f tags ext/*.*
	rm -f build/*
endif

ifeq ($(ENV),prod)

clean:
	rm -f tags
	rm -f build/*
endif
