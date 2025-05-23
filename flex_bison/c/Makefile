CC = gcc
OPT = -O3 -std=c11

# Folders
BUILD=./build
SRC=./src
INC=$(SRC)/include
TEST=./test


# Rules
all: folders builds bin tests
folders: $(BUILD)
builds: lexer parser
lexer: $(BUILD)/lexer.h $(BUILD)/lexer.c
parser: $(BUILD)/parser.h $(BUILD)/parser.c
bin: $(BUILD)/parse_file
tests: $(TEST)/result_empty.txt $(TEST)/result.txt



$(BUILD):
	test -d $@ || mkdir $@

# 
# C files generation
#
$(BUILD)/parser.h $(BUILD)/parser.c: $(SRC)/parser.y Makefile
	bison -d -v -o $(BUILD)/parser.c $(SRC)/parser.y

$(BUILD)/lexer.h $(BUILD)/lexer.c: $(SRC)/lexer.l Makefile
	flex --header-file=$(BUILD)/lexer.h -o $(BUILD)/lexer.c $(SRC)/lexer.l 

# Compilation
$(BUILD)/parse_file: $(BUILD)/lexer.c $(BUILD)/parser.c $(INC)/types.c $(INC)/types.h
	$(CC) $(OPT) -I $(INC) $^ -o $@

# Tests
$(TEST)/result.txt: $(BUILD)/parse_file $(TEST)/ressources/test.c
	$(BUILD)/parse_file $(TEST)/ressources/test.c > $(TEST)/result.txt && cat $(TEST)/result.txt

$(TEST)/result_empty.txt: $(BUILD)/parse_file $(TEST)/ressources/test_empty.c
	$(BUILD)/parse_file $(TEST)/ressources/test_empty.c > $(TEST)/result_empty.txt && cat $(TEST)/result_empty.txt


#
# Cleaning
#
clean:
	rm -f *~
	rm -f $(BUILD)/*
	rm -f $(TEST)/result*.txt


