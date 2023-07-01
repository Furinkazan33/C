#include "c_parser.h"
#include <stdio.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

int main() {
	int n;
    char *file = "int a = 2; char *my_fun(int a, int b) { \n\tif(a&&b) { return a + b; }; \n};\n";
    token *l = lex_strtok(file);
	token *p = l;

	printf("%s\n", file);

	lex_print_l(p);
	puts("");

	n = lex_count(p, T_SEMICOLON);
	token *extract = lex_extract(&p, n);
	lex_print_l(extract);
	puts("");

	n = lex_count(p, T_L_PAREN);
	extract = lex_extract(&p, n);
	lex_print_l(extract);
	puts("");

	n = lex_count(p, T_COMMA);
	extract = lex_extract(&p, n);
	lex_print_l(extract);
	puts("");

	n = lex_count(p, T_R_PAREN);
	extract = lex_extract(&p, n);
	lex_print_l(extract);
	puts("");

	puts("Remaining : ");
	lex_print_l(p);

	free(l);

    return 0;
}
