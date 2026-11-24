CC=gcc
CFLAGS=-W -Wall -Wextra -pedantic $(flags)
LDFLAGS=-I ./





LIBS_TARGETS=array db client server hashtable

all: $(addsuffix .a, $(addprefix lib,$(LIBS_TARGETS)))

define CREATE_lib_build =
$(1).o: $(1).c $(1).h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $$<

lib$(1).a: $(1).o
	ar rv $$@ $$<
	ranlib $$@
endef

$(foreach b,$(LIBS_TARGETS),$(eval $(call CREATE_lib_build,$(b))))



clean:
	rm -Rf *.a *.o

