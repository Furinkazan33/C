#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"

static FILE *FOUT;

void token_print(void *tok) {
	LEX_token *t = tok;
	char tmp[256] = {0};
	strncpy(tmp, t->start, t->len);
	fprintf(FOUT, "%s", tmp);
}

static char *readfile(const char *filename) {
	char *fcontent = NULL;
	int fsize = 0;
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		rewind(fp);

		fcontent = malloc(sizeof(char) * fsize);
		fread(fcontent, 1, fsize, fp);

		fclose(fp);
	}

	return fcontent;
}

void p_print_if_func(void *p) {
	pstruct *s = p;
	if(s->type == P_FUNC_DEF) {
		LEX_token *funcname = parser_get_funcname(s);
		printf("function found : ");
		token_print(funcname);
	}
}

int test_1(char *filepath, char *fileresult) {
	puts("");

	FOUT = fopen(fileresult, "w");

	char *text = readfile(filepath);
	arrayptr *lextokens = lexer(text);

	if(lextokens) {
		arrayptr *parser_structs = parser(lextokens);

		if(!parser_structs) {
			fprintf(stderr, "[ERROR] : no structures from parser\n");
			return 1;
		}

		// print only functions
		ARRAYPTR_MAP(parser_structs, p_print_if_func);

		//TODO free parser types

		ARRAYPTR_FREE(lextokens);
	}
	free(text);

	return 0;
}


int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s file.c file.result\n", argv[0]);
		return 1;
	}

	TEST_INIT(argv[0]);
	TESTB(1, argv[1], argv[2]);
	TEST_RESULTS();
}

