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

%token <vstring> null
%token <vstring> type
%token <vstring> _typedef
%token <vstring> _struct
%token <vstring> value_lit
%token <vstring> var_name

%type <vstring> value
%type <varray> instructions
%type <varray> instruction
%type <varray> types
%type <varray> var_decl_list
%type <varray> var_decl
%type <varray> var_decl_assign
%type <varray> var_assign
%type <varray> structure

%start file
%%

types
	: types type	{ $$ = array_add($1, $2); }
	| type		{ $$ = array_new(); $$ = array_add($$, $1); }
	;

value
	: value_lit	{ }
	| var_name	{ }
	| null		{ }
	;

var_decl_list
	: var_decl_list var_decl	{ $$ = array_add($1, $2); }
	| %empty			{ $$ = array_new(); }
	;

structure
	: _typedef _struct var_name '{' var_decl_list  '}' var_name ';' {
$$ = array_new(); $$ = array_add($$, $3); $$ = array_add($$, $5); $$ = array_add($$, $7);
 }
	;

var_assign
	: var_name '=' value ';'	{
$$ = array_new(); 
$$ = array_add($$, $1); 
$$ = array_add($$, $3); 
}
	;

var_decl
	: types var_name ';'	{ $$ = array_add($1, $2); }
	;

var_decl_assign
	: types var_name '=' value ';'	{ 
$$ = array_add($1, $2);
$$ = array_add($$, $4);
}
	;

instruction
	: var_assign 		{ printf("PARSER : var_assign\n"); }
	| var_decl_assign 	{ printf("PARSER : var_decl_assign\n"); }
	| var_decl		{ printf("PARSER : var_decl_list\n"); }
	| structure		{ printf("PARSER : structure : %s\n", $1->data[0]); }
	;

instructions
	: instructions instruction	{ $$ = array_add($1, $2); }
	| %empty			{ $$ = array_new(); }
	;

file
	: instructions		{ }
	;

%%


