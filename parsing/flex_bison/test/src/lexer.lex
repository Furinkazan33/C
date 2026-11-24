%{
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "parser.h"

%}

/*
%option debug
*/

%option noyywrap nounput noinput batch never-interactive


WS  		[ \t\v\f\n]
VALUE 		[^=\"\n]+

%%

{WS}		{	;	}

"="			{	return EQUAL; 	}


"\""		{	return QUOTE;	}

{VALUE}		{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); 
				strcpy(yylval.vstring, yytext);
				return VALUE; }

.			{ 	return 1; 	}

%%
