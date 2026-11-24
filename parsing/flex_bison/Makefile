
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
			echo $$d/Makefile; \
			make -C $$d clean; \
		fi \
	done

