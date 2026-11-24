%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "types.h"
	#include "lexer_param.h"
 
	
	void yyerror(type_array **params, const char *s) {
		fflush(stdout);
		arrays_fprint(*params, stderr);
		fprintf(stderr, "[PARSER_PARAM_ERROR] %s\n", s);
	}
	
%}


%parse-param {type_array **params}

%union{
  char *vstring;
  type_array *varray;
}

%define parse.error verbose
%locations

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

