CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


all: 
	@for d in */; do \
		if [ -e $$d/Makefile ]; then \
			make -C $$d || exit 1; \
		fi \
	done 
	@echo "" 
	@echo "" 
	@echo "               ALL COMPILATIONS OK" 
	@echo "" 
	@echo ""

clean:
	for d in */; do \
		if [ -e $$d/Makefile ]; then \
			make -C $$d clean; \
		fi \
	done
