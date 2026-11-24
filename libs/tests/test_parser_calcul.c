#include <stdlib.h>
#include <stdio.h>
#include "macros.h"
#include "arrayh.h"
#include "parser_calcul.h"

int test_1() {
	char *se1 = "3 + 4";
	char *se2 = "3 * (3 * 2) - (3 - 1)";
	
	token **tokens1 = strtokens(se1); token **p1 = tokens1;
	token **tokens2 = strtokens(se2); token **p2 = tokens2;
	//ARRAYH_MAP_ARGS(tokens, token_fprintf, stdout);
	
	size_t len = ARRAYH_CNT(p1);
	expr *e1 = tokenstoe(&p1, &len); if(!e1) { return 1; }
	ARRAYH_MAP(tokens1, token_free);
	expr_fprintf(stdout,  e1);
	simplify(e1);
	expr_fprintf(stdout,  e1);
	expr_free(e1);
	
	len = ARRAYH_CNT(p2);
	expr *e2 = tokenstoe(&p2, &len); if(!e2) { return 1; }
	ARRAYH_MAP(tokens2, token_free);
	expr_fprintf(stdout,  e2);
	simplify(e2);
	expr_fprintf(stdout,  e2);
	expr_free(e2);
	
	

	return 0;
}


int main(int argc, char **argv) {
	if(argc != 1) { printf("usage : %s\n", argv[0]); return 1; }

	if(test_1() != 0) return 1;
	


	return 0;
}

