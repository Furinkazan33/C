#include <stdlib.h>
#include <stdio.h>
#include "tnz_tpl.h"
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
	array *tokens = tnz_tpl(argv[1]);
	
	if(!tokens) {
		LOG_E("error");
		return 1;
	}


	/*	
		parser
	*/
	array *tpl = array_new(20, sizeof(p_struct));

	if(!parse_template(tpl, tokens)) {
		LOG_E("error from parser, (count=%ld)\n", tpl->count);
		pstructs_fprint(tpl, stderr);
		
		tokens_free(tokens);
		pstructs_free(tpl);
		
		return 1;
	}
	else {
		#ifdef DEBUG
			LOG_S("Result from parser :\n")
			pstructs_fprint(tpl, stdout);
		#endif

		pstructs_fprint(tpl, out);
		
		tokens_free(tokens);
		pstructs_free(tpl);

		return 0;
	}
}
