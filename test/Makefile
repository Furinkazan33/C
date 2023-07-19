CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=

DIR_LIB=./
DIR_BIN=bin
DIR_SRC=src
DIR_INC=$(DIR_SRC)/include

all: deps

deps:
	for d in */; do \
		cd $$d; \
		[ -e Makefile ] && make; \
		cd ..; \
	done

