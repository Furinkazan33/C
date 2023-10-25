%e  1019
%p  2807
%n  371
%k  284
%a  1213y
%o  1117

%option never-interactive

	
O   [0-7]
D   [0-9]
NZ  [1-9]
L   [a-zA-Z_]
A   [a-zA-Z_0-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
SP  (u8|u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))
WS  [ \t\v\n\f]

%{
#include <stdio.h>
#include <string.h>
#include "parser.h"

extern void yyerror(const char *);  /* prints grammar violation message */

#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
static char *trim_left(char *s);
static char *trim_right(char *s);

%}

%%
{WS}*"<!--"					{ comment(); }

{WS}*"<"[a-zA-Z0-9]+">"		{	yylval.vstring = trim_left(yytext); 
								//printf("BALISE_START:%s\n", yylval.vstring);
								return BALISE_START; }

"</"[a-zA-Z0-9]+">"{WS}*	{	yylval.vstring = trim_right(yytext); 
								//printf("BALISE_END:%s\n", yylval.vstring);
								return BALISE_END; }

[^<]*						{	yylval.vstring = malloc(sizeof(char) * strlen(yytext)); strcpy(yylval.vstring, yytext);
								//printf("CONTENT:%s\n", yylval.vstring);
								return CONTENT; }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}

static char *WS = " \t\v\n\f";

static char *trim_left(char *s) {
	char *res = NULL;
	while(strchr(WS, *s)) { 
		s++; 
	}
	s++; // <
	res = malloc(sizeof(char) * strlen(s));
	strncpy(res, s, strlen(s) - 1);
	res[strlen(s)] = '\0';

	return res;
}

static char *trim_right(char *s) {
	char *res = NULL;
	size_t start = 2;
	size_t end = 2;
	while(s[end] != '>') { 
		end++; 
	}

	res = malloc(sizeof(char) * (end - start + 1));
	strncpy(res, s + start, start + end - 1);
	res[end - start] = '\0';

	return res;
}

static void comment(void)
{
    int c;
	int at_least_2 = 0;

    while ((c = input()) != 0)
        if (c == '-')
        {
            while ((c = input()) == '-')
				at_least_2++;

            if (at_least_2 > 0 && c == '>')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}


