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
		fprintf(stderr, "main : file is empty\n");
		return 0;
	}



	puts("");
	puts("******************************************************************");
	puts("********************  Code to strings  ***************************");
	puts("******************************************************************");
	list *strings = lex_code_to_strings(code, &n);
	if(!strings) {
		fprintf(stderr, "main : call to lex_code_to_strings returned NULL\n");
		return 1;
	}
	list_map_void2(strings, lex_string_write, stdout);



	puts("");
	puts("******************************************************************");
	puts("********************  Strings to tokens  *************************");
	puts("******************************************************************");
	list *tokens = lex_strings_to_tokens(strings);
	if(!tokens) {
		fprintf(stderr, "main : call to lex_strings_to_tokens returned NULL\n");
		return 1;
	}
	list_map_void2(tokens, lex_token_write, stdout);



	puts("");
	puts("******************************************************************");
	puts("********************  Cleaning  **********************************");
	puts("******************************************************************");
	parser_clean(tokens, PC_NEWLINE | PC_TAB | PC_SPACE);
	list_map_void2(tokens, lex_token_write, stdout);



	puts("");
	puts("******************************************************************");
	puts("********************  Tokens to structs  *************************");
	puts("******************************************************************");
	list *structs = parser_tokens_to_structs(tokens);
	if(!structs) {
		fprintf(stderr, "main : call to parser_tokens_to_structs returned an empty list\n");
		return 1;
	}
	list_map2(structs, parser_print_statement, stdout);



	list_free_all(strings, free);
	list_free_all(tokens, lex_token_free);
	free(code);

	return 0;
}

