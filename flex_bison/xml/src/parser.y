%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "lexer.h"

 void yyerror(const char *msg);
 static int not_same(char *start, char *end);
%}

%union{
  char *vstring;
  type_balise *vbalise;
  type_attribute *vattribute;
  type_array *varray;
}

%define parse.error verbose
%locations

%code requires {
	#include "types.h"
}

%token <vstring> IDENTIFIER
%token <vstring> STRING
%token <vstring> CONTENT

%type <vbalise> balise
%type <varray> balise_list
%type <varray> attribute_list
%type <vattribute> attribute

%start file
%%

file
	: balise	{ balise_print($1); }
	;

balise
	: '<' IDENTIFIER attribute_list '>' balise_list '<' '/' IDENTIFIER '>'
		{	if(not_same($2, $8)) { return 1; }; $$ = balise_new($2, $3, $5); }
	| '<' IDENTIFIER attribute_list CONTENT  '/' IDENTIFIER '>'						
		{	if(not_same($2, $6)) { return 1; }; $$ = balise_new_atom($2, $3, $4); }
	;

balise_list
	: balise_list balise			{ $$ = array_add($1, (void *)$2); }
	|								{ $$ = array_new(); }
	;

attribute
	: IDENTIFIER '=' STRING			{ $$ = attribute_new($1, $3); }
	;

attribute_list
	: attribute_list attribute		{ $$ = array_add($1, (void *)$2); }
	|								{ $$ = array_new(); }
	;

%%

#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}

int not_same(char *start, char *end) {
	if(strcmp(start, end)) {
		fprintf(stderr, "PARSER : open tag name != close tag name : %s != %s\n", start, end);
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
   if (argc > 1) {
      yyin = fopen(argv[1], "r");
      if (yyin == NULL){
         printf("syntax: %s filename\n", argv[0]);
      }
   }
   yyparse(); // Calls yylex() for tokens.

   return 0;
}


