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

WS  [ \t\v\n\f\n]
NUM [0-9]+
ALPHA [a-zA-Z]+
ALNUM [a-zA-Z0-9]+
VARNAME [a-zA-Z_]+[0-9a-zA-Z_]*
WORD [a-zA-Z_]+
STRING_LIT \"[^"]*\"
SINGLE [;:=\{\}\[\]()]
%%

{WS}	{ ; }

"NULL" {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : NULL : %s\n", yylval.vstring);
	return null;	
}


"typedef" {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : TYPEDEF : %s\n", yylval.vstring);
	return _typedef; 
}

"struct" {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : STRUCT : %s\n", yylval.vstring);
	return _struct; 
}

"void"|"int"|"char"|"double"|"float"|"const"|"static"|"*" { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : TYPE : %s\n", yylval.vstring);
	return type; 
}

{STRING_LIT}	{ 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : STRING_LIT : %s\n", yylval.vstring);
	return value_lit; 
		}

{VARNAME} { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : VARNAME : %s\n", yylval.vstring);
	return var_name; 
	  }

{SINGLE} {
	return *yytext; 
	 }

. { 
	fprintf(stderr, "bad characters : %s", yytext); 
	return 1; 
}

%%

