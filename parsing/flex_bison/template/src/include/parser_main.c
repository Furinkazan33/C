#include <stdio.h>
#include "types.h"
#include "macros.h"
#include "parser_param.h"

extern FILE *yyin;
extern FILE *zzin;


int yyparse();
int zzparse();

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage: %s file_param file_tpl\n", argv[0]);
		return 1;
	}
		
	
	yyin = fopen(argv[1], "r");
	zzin = fopen(argv[2], "r");
	
	
	LOG_E("parsing params");
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
	
	LOG_E("parsing tpl");
	type_array *tpl;
	int rett = zzparse(&tpl, params);
	
	if(rett != 0) {
		LOG_E("zzparse error\n");
		return 1;
	}
	
	#ifdef DEBUG
		for(size_t i = 0; i < tpl->n; i++) {
			fprintf(stderr, "[%s]", (char *) tpl->data[i]);
		}
	#endif


	for(size_t i = 0; i < tpl->n; i++) {
		printf("%s", (char *) tpl->data[i]);
	}
	
	LOG_E("OK");
	
	fclose(yyin);
	fclose(zzin);
	
	return 0;
}

