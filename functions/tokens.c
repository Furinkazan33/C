#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macros.h"


char **str_tokenize(char *s, size_t tokens_init_len, char *delims, size_t *n) {
	size_t tokens_len = tokens_init_len;
	size_t n_tokens = 0;
	char **res, **tmp;

	MALLOC(res, sizeof(char *) * (tokens_len + 1), );

	for(char *t = strtok(s, delims); t; t = strtok(NULL, delims)) {
		while(n_tokens >= tokens_len) {
			tokens_len *= 2;
			REALLOC(res, tmp, sizeof(char *) * (tokens_len + 1), free(res);)
		}
		res[n_tokens++] = t;
	}
	res[n_tokens] = NULL;
	if(n != NULL) {
		*n = n_tokens;
	}
	return res;
}

int main() {
	char s[128];
	strcpy(s, "un petit oiseau bleu1,argent et\tor. ");
	char *delims = " \t\b\n-,";

	char **tokens = str_tokenize(s, 4, delims, NULL);

	for(char **p = tokens; *p; p++) {
		printf("%s\n", *p);
	}

	free(tokens);

	return 0;
}


