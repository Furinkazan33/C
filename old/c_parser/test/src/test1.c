#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

#define INIT_COLS_ALLOC 3
#define INIT_LINES_ALLOC 3

#define MESSAGE(mess) printf("%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess)
#define IF_PERROR(cond, mess, post) if(cond) { MESSAGE(mess); post; }
#define MALLOCS(var, size, post) var = malloc(size); IF_PERROR(!var, "malloc failed", post)

char *fr_readfile(char *path) {
	char *res;
	long size;
	FILE *f;

	f = fopen(path, "rb");
	IF_PERROR(!f, "fopen failed", );

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	MALLOCS(res, sizeof(char) * (size + 1), fclose(f););
	fread(res, size, 1, f);
	fclose(f);
	res[size] = 0;
	return res;
}

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
	for(list *tmp = strings; tmp != NULL; tmp = tmp->next) {
		printf("%s", (char *)tmp->data);
	}


	puts("");
	puts("******************************************************************");
	puts("********************  Strings to tokens  *************************");
	puts("******************************************************************");
	list *tokens = lex_strings_to_tokens(strings);
	if(!tokens) {
		fprintf(stderr, "main : call to lex_strings_to_tokens returned NULL\n");
		return 1;
	}
	LIST_MAP(tmp, tokens, lex_token_write);



	puts("");
	puts("******************************************************************");
	puts("********************  Cleaning  **********************************");
	puts("******************************************************************");
	parser_clean(tokens, PC_NEWLINE | PC_TAB | PC_SPACE);
	LIST_MAP(tmp, tokens, lex_token_write);



	puts("");
	puts("******************************************************************");
	puts("********************  Tokens to structs  *************************");
	puts("******************************************************************");
	list *structs = parser_tokens_to_structs(tokens);
	if(!structs) {
		fprintf(stderr, "main : call to parser_tokens_to_structs returned an empty list\n");
		return 1;
	}
	LIST_MAP(tmp, structs, parser_print_statement);



	LIST_FREE_ALL(strings, free);
	LIST_FREE_ALL(tokens, lex_token_free);
	free(code);

	return 0;
}

