CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


all: deps

deps:
	for d in */; do \
		cd $$d; \
		[ -e Makefile ] && make; \
		cd ..; \
	done

