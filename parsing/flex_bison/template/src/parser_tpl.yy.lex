%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "types.h"
	#include "macros.h"
	#include "lexer_tpl.h"

	void zzerror(type_array **tpl, type_array *params, const char *s) {
		fflush(stdout);
		LOG_E(stderr, "[PARSER_TPL_ERROR] %s\n", s);
	}
	
	
	char *get_value(char *varname, type_array *params) {
		for(size_t i = 0; i < params->n; i++) {
			type_array *a = params->data[i];
			if(!strcmp(varname, a->data[0])) {
				return a->data[1];
			}
		}
		LOG_E("varname not found in params:");
		fprintf(stderr, "%s\n", varname);
		return NULL;
	}
	
%}


%parse-param {type_array **tpl} {type_array *params}

%union{
  char *vstring;
  type_array *varray;
}

%define api.prefix {zz}

%define parse.error verbose
%locations

%token <vstring> ANY VARIABLE VAR_START1 VAR_START2 VAR_END

%type <varray> file


%start file
%%

file
	: file VAR_START1 VAR_START2 VARIABLE VAR_END	{	
							char *value = get_value($4, params);
							if(value == NULL) {
								return 1;
							}
							$$ = array_add($1, value); CHECK($$, "failed to add element"); 
							//fprintf(stderr, "PARSER:VARIABLE:[%s]\n", $4);
						}
	
	| file ANY			{	$$ = array_add($1, $2); CHECK($$, "failed to add element"); 
							//fprintf(stderr, "PARSER:ANY:[%s]\n", $2);
						}
						
	| %empty			{ 	$$ = array_new(); *tpl = $$; CHECK($$, "failed to create list"); 
							//fprintf(stderr, "PARSER:empty\n");
						}

%%


