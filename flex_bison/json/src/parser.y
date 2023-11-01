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
  char vchar;
  char *vstring;
  type_prop *vprop;
  type_value *vvalue;
  type_array *varray;
}

%define parse.error verbose
%locations

%token T_NULL
%token <vstring> IDENTIFIER 
%token <vint> BOOL
%token <vint> I_CONSTANT 
%token <vdouble> D_CONSTANT 
%token <vchar> C_CONSTANT
%token <vstring> S_CONSTANT

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
	: S_CONSTANT ':' value				{ $$ = prop_new($1, $3); }
	;

value
	: primitive							{ $$ = $1; }
	| array								{ $$ = $1; }
	| object							{ $$ = $1; }
	| T_NULL							{ $$ = NULL; }
	;

primitive
	: BOOL								{ $$ = value_new_int(INT, $1); }
	| I_CONSTANT						{ $$ = value_new_int(INT, $1); }
	| D_CONSTANT						{ $$ = value_new_double(DOUBLE, $1); }
	| C_CONSTANT						{ $$ = value_new_char(CHAR, $1); }
	| S_CONSTANT						{ $$ = value_new_string(STRING, $1); }
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


