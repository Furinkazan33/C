/* calculator. */
%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "lexer.h"
 #include "types.h"

 void yyerror(const char *msg);

%}


%union{
  double vdouble;
  int vint;
  char *vstring;
  type_prop *vprop;
  type_value *vvalue;
  type_array *varray;
}

%define parse.error verbose
%locations

%token T_NULL
%token <vstring> IDENTIFIER 
%token <vint> T_BOOL
%token <vint> T_INT 
%token <vdouble> T_DOUBLE 
%token <vstring> T_STRING

%type <vprop> property
%type <vvalue> value
%type <vvalue> primitive
%type <vvalue> array
%type <varray> array_values
%type <vvalue> object
%type <varray> object_properties

%start file
%%

file
	: object							{ value_print($1); }
	;

property
	: T_STRING ':' value				{ $$ = prop_new($1, $3); }
	;

value
	: primitive							{ $$ = $1; }
	| array								{ $$ = $1; }
	| object							{ $$ = $1; }
	;

primitive
	: T_NULL							{ $$ = NULL; }
	| T_BOOL							{ $$ = value_new_bool($1); }
	| T_INT								{ $$ = value_new_int($1); }
	| T_DOUBLE							{ $$ = value_new_double($1); }
	| T_STRING							{ $$ = value_new_string($1); }
	;

array
	: '[' array_values  ']'				{ $$ = value_new_from_array($2); }
	| '[' ']'							{ $$ = value_new_from_array(array_new()); }
	;

array_values
	: array_values ',' value			{ $$ = array_add($1, (void *)$3); }
	| value								{ $$ = array_new_from($1); }
	;

object
	: '{' object_properties '}'			{ $$ = value_new_obj_from_array($2); }
	| '{' '}'							{ $$ = value_new_obj_from_array(array_new()); }
	;

object_properties
	: object_properties ',' property	{ $$ = array_add($1, $3); }
	| property							{ $$ = array_new_from($1); }
	;

%%

#include <stdio.h>

void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
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


