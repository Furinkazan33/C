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
LIT_INT [0-9]+
LIT_FLOAT [0-9]+\.[0-9]+
SINGLE [,:=\{\}\[\]()]
TYPE "int"|"float"|"string"|"array"
%%

{WS}	{ ; }

"/*" {
	char *c = read_comments(); 
	yylval.vstring = c;
	//printf("LEXER : COMMENTS : %s\n", yylval.vstring); fflush(stdout);
	return comments;
}
"//" { 
	char *c = read_comment(); 
	yylval.vstring = c;
	//printf("LEXER : COMMENT : %s\n", yylval.vstring); fflush(stdout);
	return comment;
}

"typedef" {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : TYPEDEF : %s\n", yylval.vstring);
	return _typedef; 
}

{TYPE} { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : TYPE : %s\n", yylval.vstring);
	return type; 
}

{LIT_STRING} { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : LIT_STRING : %s\n", yylval.vstring);
	return lit_string; 
	     }

{VARNAME} { 
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : VARNAME : %s\n", yylval.vstring);
	return var_name; 
	  }

{LIT_INT} {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : LIT_INT : %s\n", yylval.vstring);
	return lit_int; 
	  }

{LIT_FLOAT} {
	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
	strcpy(yylval.vstring, yytext);
	printf("LEXER : LIT_FLOAT : %s\n", yylval.vstring);
	return lit_float; 
	  }

{SINGLE} {
	printf("LEXER : SINGLE : %c\n", *yytext);
	return *yytext; 
	 }

. { 
	fprintf(stderr, "bad characters : %s", yytext); 
	return 1; 
}

%%
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

