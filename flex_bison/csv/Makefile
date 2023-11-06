CC = gcc
OPT = -O3 -std=c11

#
# Folders
#
BUILD=./build
SRC=./src
INC=$(SRC)/include
TEST=./test


# 
# Rules
#
all: builds bin tests
builds: lexer parser
lexer: $(BUILD)/lexer.h $(BUILD)/lexer.c
parser: $(BUILD)/parser.h $(BUILD)/parser.c
bin: $(BUILD)/parse_file
tests: $(TEST)/result_empty.txt $(TEST)/result.txt


# 
# C files generation
#
$(BUILD)/parser.h $(BUILD)/parser.c: $(SRC)/parser.y Makefile
	bison -d -v -o $(BUILD)/parser.c $(SRC)/parser.y

$(BUILD)/lexer.h $(BUILD)/lexer.c: $(SRC)/lexer.l Makefile
	flex --header-file=$(BUILD)/lexer.h -o $(BUILD)/lexer.c $(SRC)/lexer.l 

#
# Compilation
#
$(BUILD)/parse_file: $(BUILD)/lexer.c $(BUILD)/parser.c $(INC)/types.c $(INC)/types.h
	$(CC) $(OPT) -I $(INC) $^ -o $@

#
# Tests
#
$(TEST)/result.txt: $(BUILD)/parse_file $(TEST)/ressources/test.csv
	$(BUILD)/parse_file $(TEST)/ressources/test.csv > $(TEST)/result.txt && cat $(TEST)/result.txt

$(TEST)/result_empty.txt: $(BUILD)/parse_file $(TEST)/ressources/test_empty.csv
	$(BUILD)/parse_file $(TEST)/ressources/test_empty.csv > $(TEST)/result_empty.txt && cat $(TEST)/result_empty.txt


#
# Cleaning
#
clean:
	rm -f *~
	rm -f $(BUILD)/*
	rm -f $(TEST)/result*.txt


