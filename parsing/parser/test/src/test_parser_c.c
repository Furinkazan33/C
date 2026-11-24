#include <stdlib.h>
#include <stdio.h>
#include "tnz_c.h"
#include "c_parser.h"
#include "macros.h"


int test_1(char *filepath, char *fileresult) {
	puts("");

	FILE *out = fopen(fileresult, "w");

	array *tokens = tnz_c(filepath);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}
	
	array *dest = array_new(20, sizeof(p_struct));

	if(!parse(&dest, tokens)) {
		LOG_E("error from parser, (count=%ld)\n", dest->count);
		ARRAY_DO(dest, idx, p_struct*, p, parser_fprint(p, stderr); );
		return 1;
	}
	
	#ifdef DEBUG
		LOG_S("Result from parser :\n")
		ARRAY_DO(dest, idx, p_struct *, p, parser_print_formated(p); );
	#endif

	ARRAY_DO(dest, idx, p_struct*, p, parser_fprint(p, out); );
	ARRAY_FREE(dest);
	ARRAY_FREE(tokens);

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s file_in file_out\n", argv[0]);
		return 1;
	}

	return test_1(argv[1], argv[2]);
	
}
