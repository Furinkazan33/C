#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../core/src/lexer.h"
#include "../../core/include/fr.h"

#define INIT_COLS_ALLOC 3
#define INIT_LINES_ALLOC 3

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "usage : %s filepath_to_load\n", argv[0]);
		return 1;
	}

	char *code = fr_readfile(argv[1]);

	token *list = lex_strtok(code);
	free(code);

	lex_print_l(list);
	free(list);


	return 0;
}
