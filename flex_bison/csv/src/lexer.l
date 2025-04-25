%{
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parser.h"

/* prints message */
extern void yyerror(const char *);  

/* called at end of input */
int yywrap(void) {
	/* terminate now */
    return 1;
}

%}

%option never-interactive

WS  [ \t\v\n\f]
NEWLINE [\n]
SEP [;]
STRING_LIT \"[^"]*\"
VALUE [^;\n]*

%%

{NEWLINE}		{ ; }

{SEP}			{	return separator; }

{STRING_LIT}	{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
				strcpy(yylval.vstring, yytext);
				printf("LEXER : STRING_LIT : %s\n", yylval.vstring);
				return value; }

{VALUE}			{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
				strcpy(yylval.vstring, yytext);
				printf("LEXER : VALUE : %s\n", yylval.vstring);
				return value; }

%%

