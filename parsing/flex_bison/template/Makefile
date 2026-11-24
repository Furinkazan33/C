
# TODO: in production, change this to ./libs
LIBS=../../../libs

CC=gcc
CFLAGS=-O3 -std=c11 ${flags}
LDFLAGS=-I $(LIBS) -I $(INC) -I src -L $(LIBS) -I $(BUILD)



# Folders
BUILD=./build
SRC=./src
INC=$(SRC)/include
TEST=./test


# Rules
#TODO: in production, uncomment this
#all: make_libs
all: folders tags builds bin tests
folders: $(BUILD)
builds: lexers parser
lexers: lexer_tpl lexer_param
lexer_tpl: $(BUILD)/lexer_tpl.h $(BUILD)/lexer_tpl.c
lexer_param: $(BUILD)/lexer_param.h $(BUILD)/lexer_param.c
parser: $(BUILD)/parser.h $(BUILD)/parser.c
bin: $(BUILD)/parse_file
tests: $(TEST)/result.txt



$(BUILD):
	test -d $@ || mkdir $@


make_libs:
	make -C libs



tags: 
	ctags -R
	
	

# C files generation
$(BUILD)/parser_param.h $(BUILD)/parser_param.c: $(SRC)/parser_param.yy.lex
	bison -d -v -o $(BUILD)/parser_param.c $^
	
$(BUILD)/parser_tpl.h $(BUILD)/parser_tpl.c: $(SRC)/parser_tpl.yy.lex
	bison -d -v -o $(BUILD)/parser_tpl.c $^

$(BUILD)/lexer_tpl.h $(BUILD)/lexer_tpl.c: $(SRC)/lexer_tpl.lex
	flex --header-file=$(BUILD)/lexer_tpl.h -o $(BUILD)/lexer_tpl.c $^

$(BUILD)/lexer_param.h $(BUILD)/lexer_param.c: $(SRC)/lexer_param.lex
	flex --header-file=$(BUILD)/lexer_param.h -o $(BUILD)/lexer_param.c $^


# Compilation
$(BUILD)/parse_file: $(BUILD)/lexer_tpl.c $(BUILD)/lexer_param.c $(BUILD)/parser_param.c $(BUILD)/parser_tpl.c $(INC)/parser_main.c $(INC)/types.c $(INC)/types.h
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


# Tests
$(TEST)/result.txt: $(BUILD)/parse_file $(TEST)/ressources/in.param $(TEST)/ressources/in.tpl
	$^ > $@
	@echo "============ Makefile ============"
	diff $(TEST)/ressources/in.tpl $@; return 0



#
# Cleaning
#
clean:
	rm -f *~
	rm -f $(BUILD)/*
	rm -f $(TEST)/result*.txt


