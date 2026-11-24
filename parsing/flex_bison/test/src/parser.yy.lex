%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "types.h"
	#include "lexer.h"
 
	void yyerror(type_array **params, const char *s) {
		fflush(stderr);
		for(size_t i = 0; i < (*params)->n; i++) {
			type_array *a = (*params)->data[i];
			fprintf(stderr, "PARSER_ERROR:%s=%s\n", (char *) a->data[0], (char *) a->data[1]);
		}
		fprintf(stderr, "PARSER_ERROR:%s\n", s);
	}
%}



/* Formatting semantic values. 
%printer { fprintf (yyo, "%s", $$); } VALUE;
%printer { fprintf (yyo, "%s=%s", (char *) $$->data[0], (char *) $$->data[1]); } param;
%printer { fprintf (yyo, "%ld", $$->n); } file;
*/

// message passed to yyerror
%define parse.error verbose

%locations


%union{
  char *vstring;
  type_array *varray;
}

%parse-param {type_array **params}

%token <vstring> VALUE
%token EQUAL QUOTE

%type <varray> file
%type <varray> param


%start file

%%

file
	: file param		{ 	$$ = array_add($1, $2); CHECK($$, "failed to add param"); } 
	| %empty			{ 	$$ = array_new(); *params = $$; CHECK($$, "failed to create params"); }
	;

param
	: VALUE EQUAL QUOTE VALUE QUOTE	{ 	$$ = array_new(); 	CHECK($$, "failed to create line"); 
										$$ = array_add($$, $1); CHECK($$, "failed to add VARNAME");
										$$ = array_add($$, $4); CHECK($$, "failed to add VALUE");	}
	;


%%

