#include <stdlib.h>
#include "fr.h"
#include "macros.h"

char *fr_readline(FILE *f, char *dest, size_t *len) {
	char c;
	*len = 0;
	while((c = fgetc(f)) != EOF) {
		dest[(*len)++] = c;
		if(c == '\n') 
			break;
	}
	dest[*len] = '\0';

	return dest;
}

char *fr_readfile(char *path) {
	char *res;
	long size;
	FILE *f;

	f = fopen(path, "rb");
	if(!f) { perror(""); LOGE("fopen failed"); return NULL; }

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	MALLOC(res, sizeof(char) * (size + 1), fclose(f););
	fread(res, size, 1, f);
	fclose(f);
	res[size] = 0;
	return res;
}

