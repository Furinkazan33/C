#include <stdio.h>
#include "types.h"
#include "parser.h"

extern FILE *yyin;


int yyparse();


int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "usage: %s file\n", argv[0]);
		return 1;
	}
		
	
	yyin = fopen(argv[1], "r");
	
	
	LOG_E("parsing params\n");
	type_array *params;
	int retp = yyparse(&params);
	
	if(retp != 0) {
		LOG_E("yyparse error\n");
		return 1;
	}
	
	#ifdef DEBUG
		for(size_t i = 0; i < params->n; i++) {
			type_array *a = params->data[i];
			fprintf(stderr, "%s=", (char *) a->data[0]);
			fprintf(stderr, "%s\n", (char *) a->data[1]);
		}
	#endif
	
	
	LOG_E("OK\n");
	
	fclose(yyin);
	
	return 0;
}

