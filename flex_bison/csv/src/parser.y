/* calculator. */
%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "lexer.h"
 #include "types.h"

 void yyerror(const char *msg);

%}


%union{
  char *vstring;
  type_array *varray;
}

%define parse.error verbose
%locations

%token <vstring> ELT

%type <varray> lines
%type <varray> line
%type <varray> elt_list

%start file
%%

file
	: lines					{ lines_print((void *)$1); }
	;

lines
	: lines line			{ $$ = array_add($1, $2); }
	|						{ $$ = array_new(); }
	;

line
	: elt_list ELT		{ $$ = array_add($1, $2); }
	;

elt_list
	: elt_list ELT ';'		{ $$ = array_add($1, $2); }
	|						{ $$ = array_new(); }
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


