CC=gcc
CFLAGS=-W -Wall -Wextra -pedantic -Wpadded $(flags)
LDFLAGS=-I libs -L libs


TARGETS=flags hashmap wordexp static_result server client

flags_srcs=src/test_flags.c
flags_deps=
flags_libs=

hashmap_srcs=src/test_hashmap.c
hashmap_deps=
hashmap_libs=

wordexp_srcs=src/test_wordexp.c
wordexp_deps=
wordexp_libs=

static_result_srcs=src/test_static_result.c
static_result_deps=
static_result_libs=

server_srcs=src/test_server.c
server_deps=libs/libserver.a
server_libs=-lserver

client_srcs=src/test_client.c
client_deps=libs/libclient.a
client_libs=-lclient


all: $(addprefix build/,$(TARGETS))


libs/lib%.a: libs/%.c libs/%.h
	make -C libs $(@F)


define CREATE_build =
$(addprefix build/,$(1)): $(2) $(3)
	$(CC) $(CFLAGS) $(LDFLAGS) $$< $(4) -o $$@
endef


$(foreach b,$(TARGETS),$(eval $(call CREATE_build,\
$(b),\
$($(b)_srcs),\
$($(b)_deps),\
$($(b)_libs))))


clean:
	make -C libs clean
	rm -Rf build/*

