#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char **argv) {
	if (argc > 1) {
		yyin = fopen(argv[1], "r");
		if (yyin == NULL){
			printf("syntax: %s filename\n", argv[0]);
		}
	}
	int res = yyparse(); // Calls yylex() for tokens.

	fclose(yyin);

	return res;
}

