#include <stdlib.h>
#include <stdio.h>
#include "tnz_param.h"
#include "tnz_tpl.h"
#include "parser.h"
#include "macros.h"


int parse_and_replace(char *filepath_params, char *filepath_tpl, char *filepath_result) {
	FILE *fres = fopen(filepath_result, "w");

	array *tokens_params = tnz_param(filepath_params);
	if(!tokens_params) {
		LOG_E("error");
		return 1;
	}
	
	array *tokens_tpl = tnz_tpl(filepath_tpl);
	if(!tokens_tpl) {
		LOG_E("error");
		return 1;
	}
	
	array *params = array_new(20, sizeof(p_struct));
	array *tpl = array_new(20, sizeof(p_struct));

	if(!parse_param(params, tokens_params)) {
		LOG_E("error from parser, (count=%ld)\n", params->count);
		
		pstructs_fprint(params, stderr);
		
		return 1;
	}
	#ifdef DEBUG
		LOG_S("Result from parser :\n")
		pstructs_print_formated(params);
	#endif
	
	if(!parse_template(tpl, tokens_tpl)) {
		LOG_E("error from parser, (count=%ld)\n", tpl->count);
		pstructs_fprint(tpl, stderr);

		return 1;
	}
	#ifdef DEBUG
		LOG_S("Result from parser :\n")
		pstructs_print_formated(tpl);
	#endif


	if(!parse_replace(params, tpl)) {
		LOG_E("error from parser\n");
		pstructs_fprint(tpl, stderr);

		return 1;
	}
	#ifdef DEBUG_REPLACE
		LOG_S("Result from parser :\n")
		pstructs_print_formated(tpl);
	#endif

	pstructs_fprint(tpl, fres);

	fclose(fres);
	
	pstructs_free(params);
	pstructs_free(tpl);
	tokens_free(tokens_params);
	tokens_free(tokens_tpl);

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 4) {
		fprintf(stderr, "usage : %s filepath_params filepath_tpl filepath_result\n", argv[0]);
		return 1;
	}


	char *filepath_params = argv[1];
	char *filepath_tpl = argv[2];
	char *filepath_result = argv[3];

	return parse_and_replace(filepath_params, filepath_tpl, filepath_result);
}
