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
  type_array *varray;
}

%define parse.error verbose
%locations

%code requires {
	#include "types.h"
}

%token <vstring> BALISE_START
%token <vstring> CONTENT
%token <vstring> BALISE_END
%type <vbalise> balise
%type <varray> balise_list

%start file
%%

file
	: balise								{	balise_print($1); }
	;

balise
	: BALISE_START balise_list BALISE_END	{	if(not_same($1, $3)) { 
													return 1; 
												}; 
												$$ = balise_new($1, $2); 
											}

	| BALISE_START CONTENT BALISE_END		{	if(not_same($1, $3)) { 
													return 1; 
												}; 
												$$ = balise_new_atom($1, $2); 
											}
	;

balise_list
	: balise_list balise					{ $$ = array_add($1, (void *)$2); }
	|										{ $$ = array_new(); }
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


