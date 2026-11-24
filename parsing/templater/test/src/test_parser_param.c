#include <stdlib.h>
#include <stdio.h>
#include "tnz_param.h"
#include "parser.h"
#include "macros.h"



int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage : %s filepath_params filepath_result\n", argv[0]);
		return 1;
	}

	FILE *out = fopen(argv[2], "w");


	/*
		lexer
	*/
	array *tokens = tnz_param(argv[1]);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}


	/*	
		parser
	*/
	array *params = array_new(20, sizeof(p_struct));

	if(!parse_param(params, tokens)) {
		LOG_E("error from parser, (count=%ld)\n", params->count);
		pstructs_fprint(params, stderr);
		
		tokens_free(tokens);
		pstructs_free(params);
		
		return 1;
	}
	else {
		#ifdef DEBUG
			LOG_S("Result from parser :\n")
			pstructs_fprint(params, stdout);	
		#endif

		pstructs_fprint(params, out);
		
		tokens_free(tokens);
		pstructs_free(params);

		return 0;
	}
}
