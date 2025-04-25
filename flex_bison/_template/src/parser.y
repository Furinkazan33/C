%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "lexer.h"
 #include "types.h"

void yyerror(const char *s) {
	fflush(stdout);
	fprintf(stderr, "[ERROR] %s\n", s);
}
int yyparse();


static type_array *global_type_defs = NULL;

type_array *global_find_type_def(char *var_name) {
	for(size_t i = 0; i < global_type_defs->n; i++) {
		type_array *cur = global_type_defs->data[i];
		if(!strcmp(var_name, (char *) cur->data[0])) { 
			return cur;
		}
	}
	return NULL;
}

%}


%union{
  char *vstring;
  type_array *varray;
}

%define parse.error verbose
%locations

%token <vstring> comment
%token <vstring> comments
%token <vstring> type
%token <vstring> _typedef
%token <vstring> lit_string
%token <vstring> lit_int
%token <vstring> lit_float
%token <vstring> var_name

%type <vstring> value
%type <varray> instructions
%type <varray> instruction
%type <varray> value_list1
%type <varray> value_list
%type <varray> var_decl_list1
%type <varray> var_decl_list
%type <varray> var_decl
%type <varray> var_assign
%type <varray> type_def

%start file
%%


// non-empty list
var_decl_list1
	: var_decl_list1 ',' var_decl 	{ $$ = array_add($1, $3); }
	| var_decl			{ $$ = array_new(); $$ = array_add($$, $1); }
	;

var_decl_list
	: var_decl_list1 		{ $$ = $1; }
	| %empty			{ $$ = array_new(); }
	;

// non-empty list
value_list1
	: value_list1 ',' value	{ $$ = array_add($1, $3); }
	| value			{ $$ = array_new(); $$ = array_add($$, $1); }
	;

value_list
	: value_list1		{ $$ = $1; }
	| %empty		{ $$ = array_new(); }
	;

lit_object
	: '{' value_list '}'
	;

lit_array
	: '[' value_list ']'
	;

value
	: lit_string	{ }
	| lit_int	{ }
	| lit_float	{ }
	| lit_object	{ }
	| lit_array	{ }
	| var_name	{ }
	;

type_def
	: _typedef var_name '(' var_decl_list ')' { 
$$ = array_new(); 
$$ = array_add($$, $2); 
$$ = array_add($$, $4); 

if(!global_type_defs) { global_type_defs = array_new(); }
array_add(global_type_defs, $$);
}
	;

// TODO: check types
var_assign
	: var_name '=' value	{ $$ = array_new(); $$ = array_add($$, $1); $$ = array_add($$, $3); }
	| var_decl '=' value 	{ $$ = $1; $$ = array_add($$, $3); }
	;

var_decl
	: type var_name 	{ $$ = array_new(); $$ = array_add($$, $1); $$ = array_add($$, $2); }
	| var_name var_name 	{
// checking that type as var_name is defined
//if(!global_type_defs) { fprintf(stderr, "[ERROR] array not initialized\n"); return 1; }
if(!global_find_type_def($1)) { fprintf(stderr, "[ERROR] type inexistant : %s\n", $1); return 1; }

$$ = array_new(); 
$$ = array_add($$, $1); 
$$ = array_add($$, $2); 
}
	;

instruction
	: comment 		{ printf("PARSER : comment\n"); }
	| comments 		{ printf("PARSER : comments\n"); }
	| var_decl		{ printf("PARSER : var_decl\n"); }
	| var_assign		{ printf("PARSER : var_assign\n"); }
	| type_def		{ printf("PARSER : type_def\n"); }
	;

instructions
	: instructions instruction	{ $$ = array_add($1, $2); }
	| %empty			{ $$ = array_new(); }
	;

file
	: instructions		{ }
	;

%%



