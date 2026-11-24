
# TODO: in production, change this to ./libs
LIBS=../../../libs


CC=gcc
CFLAGS=-O3 -std=c11 ${flags}
LDFLAGS= -I $(LIBS) -L $(LIBS) -I $(INC) -I $(BUILD) 

# Folders
BUILD=./build
SRC=./src
INC=$(SRC)/include
TEST=./test


# Rules
all: folders lexer parser bin tests
folders: $(BUILD)
lexer: $(BUILD)/lexer.h $(BUILD)/lexer.c
parser: $(BUILD)/parser.h $(BUILD)/parser.c
bin: $(BUILD)/parse_file
tests: $(TEST)/result.txt



$(BUILD):
	test -d $@ || mkdir $@


# C files generation
$(BUILD)/parser.h $(BUILD)/parser.c: $(SRC)/parser.yy.lex
	bison -Dparse.trace=true -d -v -o $(BUILD)/parser.c $^

$(BUILD)/lexer.h $(BUILD)/lexer.c: $(SRC)/lexer.lex
	flex --header-file=$(BUILD)/lexer.h -o $(BUILD)/lexer.c $^


# Compilation
$(BUILD)/parse_file: $(BUILD)/lexer.c $(BUILD)/parser.c $(INC)/parser_main.c $(INC)/types.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@


# Tests
$(TEST)/result.txt: $(BUILD)/parse_file $(TEST)/ressources/in.param
	$^



#
# Cleaning
#
clean:
	rm -f *~
	rm -f $(BUILD)/*
	rm -f $(TEST)/result*.txt


