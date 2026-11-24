# for debug mode : make -BE flags=-DDEBUG
CC=gcc
CFLAGS=-W -Wall -ansi -std=c17 -Wextra -pedantic $(flags)
LDFLAGS=-I libs -I src/ -L libs



#
# main targets
#
all: all_ok



#
# libs
#
libs/%.a:
	make -C libs $(@F)


#
# .o files
#
O_FILES=tokenizer tnz_default tnz_c tnz_html c_parser

define CREATE_o_file =
build/$(1).o: src/$(1).c src/$(1).h
	$(CC) $(CFLAGS) $(LDFLAGS) -c $$< -o $$@
endef

$(foreach o_file,$(O_FILES),$(eval $(call CREATE_o_file,$(o_file))))



#
# Tests lexers
#

# c as text
COMP_LEX_DEF=test/src/test_tokenizer.c build/tokenizer.o build/tnz_default.o
test/build/test_tokenizer: $(COMP_LEX_DEF) libs/libarray.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(COMP_LEX_DEF) -larray -o $@

test/results/tokenizer_out.c: ./test/build/test_tokenizer ./test/ressources/file.c
	$^ $@

test/results/tokenizer_ok: ./test/ressources/file.c test/results/tokenizer_out.c
	rm -f $@; diff $^ && touch $@;


# c as c code
COMP_LEX_C=test/src/test_tokenizer_c.c build/tokenizer.o build/tnz_c.o
test/build/test_tokenizer_c: $(COMP_LEX_C) libs/libarray.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(COMP_LEX_C) -larray -o $@

test/results/tokenizer_c_out.c: ./test/build/test_tokenizer_c ./test/ressources/file.c
	$^ $@

test/results/tokenizer_c_ok: ./test/ressources/file.c test/results/tokenizer_c_out.c  
	rm -f $@; diff $^ && touch $@;


# html
COMP_LEX_HTML=test/src/test_tokenizer_html.c build/tokenizer.o build/tnz_html.o
test/build/test_tokenizer_html: $(COMP_LEX_HTML) libs/libarray.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(COMP_LEX_HTML) -larray -o $@

test/results/tokenizer_html_out.html: ./test/build/test_tokenizer_html ./test/ressources/file.html
	$^ $@

test/results/tokenizer_html_ok: ./test/ressources/file.html test/results/tokenizer_html_out.html
	rm -f $@; diff $^ && touch $@;


# lexers results
lexers_ok: test/results/tokenizer_ok test/results/tokenizer_c_ok test/results/tokenizer_html_ok
	@echo "------------------ ALL LEXERS OK ------------------"




#
# Tests parsers
#

# c
COMP_PARSER=test/src/test_parser_c.c build/tokenizer.o build/tnz_c.o build/c_parser.o
test/build/test_parser_c: $(COMP_PARSER) libs/libarray.a
	$(CC) $(CFLAGS) $(LDFLAGS) $(COMP_PARSER) -larray -o $@

test/results/parser_c_out.c: ./test/build/test_parser_c ./test/ressources/file.c
	$^ $@

test/results/parser_c_ok: ./test/ressources/file.c test/results/parser_c_out.c  
	rm -f $@; diff $^ && touch $@;


# parsers results
parsers_ok: test/results/parser_c_ok
	@echo "------------------ ALL PARSERS OK ------------------"




all_ok: lexers_ok parsers_ok
	@echo "------------------ ALL RESULTS OK ------------------"




#
# clean
#
clean:
	make -C libs clean
	rm -f build/*;
	rm -f test/build/*;
	rm -f test/results/*;

