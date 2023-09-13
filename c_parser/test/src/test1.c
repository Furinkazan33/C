#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "fr.h"

#define INIT_COLS_ALLOC 3
#define INIT_LINES_ALLOC 3


int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "usage : %s filepath_to_load\n", argv[0]);
		return 1;
	}
	int n;
	char *code = fr_readfile(argv[1]);
	if(!code) {
		fprintf(stderr, "file is empty\n");
		return 0;
	}

	list *strings = lex_code_to_strings(code, &n);
	if(!strings) {
		fprintf(stderr, "call to lex_code_to_strings returned NULL\n");
		return 1;
	}
	//list_map2(strings, lex_string_write, stdout);

	puts("");
	puts("******************************************************************");
	puts("******************************************************************");
	puts("******************************************************************");

	list *tokens = lex_strings_to_tokens(strings);
	if(!tokens) {
		fprintf(stderr, "call to lex_strings_to_tokens returned NULL\n");
		return 1;
	}
	//list_map2(tokens, lex_token_write, stdout);

	puts("");
	puts("******************************************************************");
	puts("******************************************************************");
	puts("******************************************************************");

	tokens = parser_clean(tokens, PC_NEWLINE | PC_TAB | PC_SPACE);
	if(!tokens) {
		fprintf(stderr, "call to parser_clean returned an empty list\n");
		return 1;
	}

	list_map2(tokens, lex_token_write, stdout);

	list_free_all(strings, free);
	list_free_all(tokens, lex_token_free);
	free(code);

	return 0;
}




