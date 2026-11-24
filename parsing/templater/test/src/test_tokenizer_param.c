#include <stdlib.h>
#include <stdio.h>
#include "tnz_param.h"
#include "tokenizer.h"
#include "macros.h"


int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s file_in file_out\n", argv[0]);
		return 1;
	}

	FILE *out = fopen(argv[2], "w");

	array *tokens = tnz_param(argv[1]);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}

	tokens_fprint(tokens, out);
	tokens_free(tokens);

	return 0;
}
