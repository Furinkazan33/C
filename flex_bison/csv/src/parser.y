%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "lexer.h"
 #include "types.h"
 #include "parser_main.c"
%}


%union{
  char *vstring;
  type_array *varray;
}

%define parse.error verbose
%locations

%token <vstring> value
%token separator

%type <varray> lines
%type <varray> line

%start file
%%

file
	: lines				{ 	lines_print((void *)$1); }
	;

lines
	: lines line			{ 	$$ = array_add($1, $2); CHECK($$, "failed to add line"); } 
	| %empty			{ 	$$ = array_new(); 	CHECK($$, "failed to create lines"); }
	;

line
	: line separator value 		{ 	$$ = array_add($1, $3); CHECK($$, "failed to add elt"); }
	| value 			{ 	$$ = array_new(); 	CHECK($$, "failed to create line"); 
						$$ = array_add($$, $1); CHECK($$, "failed to add first elt"); }
	;

%%


