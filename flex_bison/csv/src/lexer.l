%option never-interactive

WS  [ \t\v\n\f]

%{
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parser.h"

extern void yyerror(const char *);  /* prints grammar violation message */

%}

%%

[\n]		{ ; }

[;]			{	return *yytext; }

[^;\n]*		{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
				strcpy(yylval.vstring, yytext);
				//printf("LEXER : ELT : %s\n", yylval.vstring);
				return ELT; }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}


