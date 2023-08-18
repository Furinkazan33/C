CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c18 -D_POSIX_C_SOURCE=200809L
LDFLAGS=


all: compile
	
compile: .ONESHELL exit

.ONESHELL:
	@echo "Looking for Makefiles in children folders.";
	@found=0
	@for d in $$(ls); do
		if [ -d $$d ]; then
			@printf "$$d => folder found\n"
	       		if [ -f $$d/Makefile ]; then
				found=1
				@printf "$$d/Makefile => Makefile found\n"
		       		make -C $$d;
			fi;
		fi;
	done;
	@if [ $$found -eq 0 ]; then
		printf "No Makefile found !\n"
	else
		echo "" 
		echo "" 
		echo "               ALL TESTS COMPILATIONS OK" 
		echo "" 
		echo ""
	fi

exit: .ONESHELL
	@exit 0
