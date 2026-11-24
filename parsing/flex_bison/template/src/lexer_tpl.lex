%{
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parser_tpl.h"

/* prints message */
extern void zzerror(const char *);  

/* called at end of input */
int zzwrap(void) {
	/* terminate now */
    return 1;
}

%}

%option prefix="zz"

%option never-interactive

VARIABLE		[a-zA-Z_]+[0-9a-zA-Z_]*
VAR_START1		"$"
VAR_START2		"{"
VAR_END			"}"
ANY				[^${}]+

%%


{VAR_START1}	{	
					#ifdef DEBUG
						fprintf(stderr, "LEXER:VAR_START1:[%c]\n", *yytext);
					#endif
					return VAR_START1;	}

{VAR_START2}	{	
					#ifdef DEBUG
						fprintf(stderr, "LEXER:VAR_START2:[%c]\n", *yytext);
					#endif
					return VAR_START2;	}

{VAR_END}		{	
					#ifdef DEBUG
						fprintf(stderr, "LEXER:VAR_END:[%c]\n", *yytext);
					#endif
					return VAR_END;	}

{VARIABLE}		{	zzlval.vstring = malloc(sizeof(char) * (strlen(zztext) + 1)); 
					strcpy(zzlval.vstring, zztext);
					#ifdef DEBUG
						fprintf(stderr, "LEXER:VARIABLE:[%s]\n", zzlval.vstring);
					#endif
					return VARIABLE; }

{ANY}			{	zzlval.vstring = malloc(sizeof(char) * (strlen(zztext) + 1)); 
					strcpy(zzlval.vstring, zztext);
					#ifdef DEBUG
						fprintf(stderr, "LEXER:ANY:[%s]\n", zzlval.vstring);
					#endif
					return ANY; }
					
.				{ 
					fprintf(stderr, "bad characters:[%s]", yytext); 
					return 1; 
				}
%%

