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
  type_section *vsection;
  type_array *varray;
}

%define parse.error verbose
%locations

%token <vstring> IDENTIFIER 
%token <vint> I_CONSTANT 
%token <vdouble> D_CONSTANT 
%token <vchar> C_CONSTANT
%token <vstring> S_CONSTANT

%type <varray> file_slist
%type <vsection> section
%type <varray> section_plist
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
	: file_slist						{ array_map($1, section_print); }
	;

file_slist
	: file_slist section				{ $$ = array_add($1, (void *)$2); }
	|									{ $$ = array_new(); }
	;

section
	: '[' IDENTIFIER  ']' section_plist	{ $$ = section_new($2, $4); }
	;

section_plist
	: section_plist property			{ $$ = array_add($1, (void *)$2); }
	|									{ $$ = array_new(); }
	
property
	: IDENTIFIER '=' value				{ $$ = prop_new($1, $3); }
	;

value
	: primitive							{ $$ = $1; }
	| array								{ $$ = $1; }
	| object							{ $$ = $1; }
	| IDENTIFIER						{ $$ = value_new_from_varname($1); }
	;

primitive
	: I_CONSTANT						{ $$ = value_new_int(INT, $1); }
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


