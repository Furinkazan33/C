#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

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


int test_1(char *filepath, char *fileresult) {
	puts("");

	FOUT = fopen(fileresult, "w");

	char *text = readfile(filepath);
	arrayptr *tokens = lexer(text);
	if(tokens) {
		ARRAYPTR_MAP(tokens, token_print);

		ARRAYPTR_FREE(tokens);
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

