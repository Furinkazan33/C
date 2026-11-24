CC=gcc
OPT=-O3 -std=c11

# Folders
BUILD=./build
SRC=./src
INC=$(SRC)/include
TEST=./test


# Rules
all: folders objects bin tests
folders: $(BUILD)
objects: $(BUILD)/lexer.o $(BUILD)/parser.o
lexer.o: $(BUILD)/lexer.h $(BUILD)/lexer.c
parser.o: $(BUILD)/parser.h $(BUILD)/parser.c
bin: $(BUILD)/parse_file
tests: $(TEST)/result_empty.txt $(TEST)/result.txt



$(BUILD):
	test -d $@ || mkdir $@

# C files generation
$(BUILD)/parser.h $(BUILD)/parser.c: $(SRC)/parser.y Makefile
	bison -d -v -o $(BUILD)/parser.c $(SRC)/parser.y

$(BUILD)/lexer.h $(BUILD)/lexer.c: $(SRC)/lexer.l $(BUILD)/parser.h Makefile
	flex --header-file=$(BUILD)/lexer.h -o $(BUILD)/lexer.c $(SRC)/lexer.l 

# objects
$(BUILD)/lexer.o: $(BUILD)/lexer.c $(INC)/types.c $(INC)/types.h
	$(CC) $(OPT) -I $(INC) -I $(BUILD) -c $< -o $@

$(BUILD)/parser.o: $(BUILD)/parser.c $(INC)/types.c $(INC)/types.h
	$(CC) $(OPT) -I $(INC) -I $(BUILD) -c $< -o $@

# bin
$(BUILD)/parse_file: $(BUILD)/parser.o $(BUILD)/lexer.o $(INC)/types.c $(INC)/types.h $(SRC)/main.c
	$(CC) $(OPT) -I $(INC) -I $(BUILD) $^ -o $@


# Tests
$(TEST)/result.txt: $(BUILD)/parse_file $(TEST)/ressources/test.box
	$(BUILD)/parse_file $(TEST)/ressources/test.box > $(TEST)/result.txt; cat $(TEST)/result.txt

$(TEST)/result_empty.txt: $(BUILD)/parse_file $(TEST)/ressources/test_empty.box
	$(BUILD)/parse_file $(TEST)/ressources/test_empty.box > $(TEST)/result_empty.txt && cat $(TEST)/result_empty.txt


# Cleaning
clean:
	rm -f *~
	rm -f $(BUILD)/*
	rm -f $(TEST)/result*.txt


