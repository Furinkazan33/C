%{
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parser_param.h"

/* prints message */
extern void yyerror(const char *);  

/* called at end of input */
int yywrap(void) {
	/* terminate now */
    return 1;
}

%}

%option never-interactive

WS  		[ \t\v\f\n]
VALUE 		[^=\"\n]+

%%

{WS}		{	;	}

"="			{	
				#ifdef DEBUG
					fprintf(stderr, "LEXER:EQUAL:[%s]\n", yytext);	
				#endif
				return EQUAL; 
			}


"\""		{
				#ifdef DEBUG
					fprintf(stderr, "LEXER:QUOTE:[%s]\n", yytext); 
				#endif
				return QUOTE;
			}

{VALUE}		{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
				strcpy(yylval.vstring, yytext);
				#ifdef DEBUG
					fprintf(stderr, "LEXER:VALUE:[%s]\n", yylval.vstring);
				#endif
				return VALUE; }

.			{ 
				#ifdef DEBUG
					fprintf(stderr, "bad characters:[%s]", yytext); 
				#endif
				return 1; 
			}

%%

