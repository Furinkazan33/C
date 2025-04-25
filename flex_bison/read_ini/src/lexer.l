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

static void ignore_ws();
static char *read_comments(void);
static char *read_comment(void);

%}

%option never-interactive

WS  [ \t\v\n\f\n]
NUM [0-9]+
ALPHA [a-zA-Z]+
ALNUM [a-zA-Z0-9]+
VARNAME [a-zA-Z_]+[0-9a-zA-Z_]*
WORD [a-zA-Z_]+
LIT_STRING \"[^"]*\"
SINGLE [=\[\]]
%%

{WS}	{
	//printf("LEXER : WS\n"); fflush(stdout);
}

";" { 
	char *c = read_comment(); 
	yylval.vstring = c;
	//printf("LEXER : COMMENT : %s\n", yylval.vstring); fflush(stdout);
	return comment;
}

{VARNAME} { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	//printf("LEXER : VARNAME : %s\n", yylval.vstring);
	return var_name; 
	  }

{SINGLE} {
	//printf("LEXER : SINGLE : %c\n", *yytext);
	return *yytext; 
	 }

={WS}*.*\n {
	yytext++; // ignore '='
	ignore_ws();
	
	size_t len = strlen(yytext) - 1;
	yylval.vstring = malloc(sizeof(char) * (len + 1)); 
	strncpy(yylval.vstring, yytext, len);
	yylval.vstring[len] = '\0';
	//printf("LEXER : VALUE : %s\n", yylval.vstring);
	return value; 
	  }


%%

static void ignore_ws() {
	while(*yytext == ' ' || *yytext == '\t') {
		yytext++;
	}
}
static char *read_comment(void)
{
	int c;
	size_t size = 32;
	char *res = malloc(sizeof(char) * (size + 1));

	for(size_t i = 0; (c = input()) != 0 && c != '\n'; i++) {
		if(i >= size) {
			size *= 2;
			res = realloc(res, sizeof(char) * (size + 1));
		}
		res[i] = c;
	}
	if(!c) {
		yyerror("unterminated comment");
	}

	return res;
}

static char *read_comments(void)
{
	int c, next;
	size_t size = 32;
	char *res = malloc(sizeof(char) * (size + 1));

	for(size_t i = 0; (c = input()) != 0 && (c != '*' || (next = input()) != '/'); i++) {
		if(i >= size) {
			size *= 2;
			res = realloc(res, sizeof(char) * (size + 1));
		}
		res[i] = c;
	}
	if(!c) {
		yyerror("unterminated comment");
	}

	return res;
}

