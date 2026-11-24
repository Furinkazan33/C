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

%}

%%
{WS}*				{	//printf("WS\n"); 
						}

"<!--"				{	comment(); }

[<>/=]				{	//printf("char : %c\n", *yytext); 
						return *yytext;  }

{L}+{A}*			{	yylval.vstring = malloc(sizeof(char) * strlen(yytext)); strcpy(yylval.vstring, yytext); 
						//printf("IDENTIFIER : %s\n", yylval.vstring);
						return IDENTIFIER; }

\"[^"]*\"			{	size_t len = strlen(yytext) - 2;
						yylval.vstring = malloc(sizeof(char) * (len + 1)); 
						strncpy(yylval.vstring, yytext + 1, len);
						yylval.vstring[len] = '\0';
						//printf("STRING     : %s\n", yylval.vstring);
						return STRING; }

">"(.)*"<"			{	size_t len = strlen(yytext) - 2;
						yylval.vstring = malloc(sizeof(char) * (len + 1)); 
						strncpy(yylval.vstring, yytext + 1, len);
						yylval.vstring[len] = '\0';
						//printf("CONTENT    : %s\n", yylval.vstring);
						return CONTENT; }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
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


