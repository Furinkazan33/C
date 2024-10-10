#include <stdlib.h>
#include "fr.h"
#include "macros.h"

char *fr_readnextline(FILE *f, size_t alloc, size_t realloc_coef, int *n_read) {
	char *res;
	char c;
	size_t i;

	MALLOC(res, sizeof(char) * (alloc + 1), );

	c = fgetc(f);
	for(i = 0; c != EOF && c !='\0'; i++) {
		if(i >= alloc) {
			alloc *= realloc_coef;
			res = realloc(res, sizeof(char) * (alloc + 1));
			IF_PERROR(!res, "realloc failed", );
		}
		if(c == '\n') { break; }
		res[i] = c;
		c = fgetc(f);
	}
	res[i] = '\0';
	if(c == '\n') { i++; }
	*n_read = i;

	return res;
}

char *fr_readfile(char *path) {
	char *res;
	long size;
	FILE *f;

	f = fopen(path, "rb");
	IF_PERROR(!f, "fopen failed", );

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	MALLOC(res, sizeof(char) * (size + 1), fclose(f););
	fread(res, size, 1, f);
	fclose(f);
	res[size] = 0;
	return res;
}

