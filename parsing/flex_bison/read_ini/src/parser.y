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


%}


%union{
  char *vstring;
  type_array *varray;
}

%define parse.error verbose
%locations

%token <vstring> comment
%token <vstring> comments
%token <vstring> lit_string
%token <vstring> var_name
%token <vstring> value

%type <varray> file
%type <varray> group
%type <varray> properties
%type <varray> property

%start file
%%

property
	: comment			{ printf("PARSER : group comment [%s]\n", $1); }
	| var_name value	{ 
$$ = array_new(); 
$$ = array_add($$, $1); 
$$ = array_add($$, $2); 
printf("PARSER : property %s=%s\n", $1, $2);
}
	;

properties
	: properties property	{ $$ = array_add($1, $2); }
	| %empty				{ $$ = array_new(); }
	;

group
	: '[' var_name ']' properties { 
$$ = array_new(); 
$$ = array_add($$, $2); 
$$ = array_add($$, $4); 
printf("PARSER : group %s\n", $2);
}
	;


file
	: file group		{ $$ = array_add($$, $1); }
	| file comment		{ printf("PARSER : file comment : [%s]\n", $2); }
	| %empty			{ $$ = array_new(); }
	;

%%



