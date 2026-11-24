#include <stdlib.h>
#include <stdio.h>
#include "tokenizer.h"
#include "tnz_tpl.h"
#include "macros.h"


int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s file_in file_out\n", argv[0]);
		return 1;
	}

	char *filepath = argv[1];
	char *fileresult = argv[2];
	
	FILE *out = fopen(fileresult, "w");

	array *tokens = tnz_tpl(filepath);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}

	tokens_fprint(tokens, out);
	tokens_free(tokens);

	return 0;
}
