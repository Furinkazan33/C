#include <stdlib.h>
#include <stdio.h>
#include "tnz_default.h"
#include "macros.h"

int test_1(char *filepath, char *fileresult) {
	puts("");

	FILE *out = fopen(fileresult, "w");

	array *tokens = tnz_default(filepath);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}

	tnz_default_fprint(tokens, out);
	tnz_default_free(tokens);

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s file_in file_out\n", argv[0]);
		return 1;
	}

	return test_1(argv[1], argv[2]);
}
