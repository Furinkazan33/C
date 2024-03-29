%e  1019
%p  2807
%n  371
%k  284
%a  1213
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
#include "types.h"
#include "parser.h"

extern void yyerror(const char *);  /* prints grammar violation message */

#define sym_type(identifier) IDENTIFIER /* with no symbol table, fake it */

static void comment(void);
%}

%%
"/*"                    { comment(); }
"//".*                  { /* consume comment line */ }

"true"					{   yylval.vint = 1; return T_BOOL; }
"false"					{   yylval.vint = 0; return T_BOOL; }
"null"					{	return T_NULL; }

{L}{A}*					{	yylval.vstring = malloc(sizeof(char) * (strlen(yytext) + 1)); strcpy(yylval.vstring, yytext);
							return IDENTIFIER; }

{D}+					{	yylval.vint = atoi(yytext); 
							return T_INT; }

{D}*\.{D}+				{	yylval.vdouble = atof(yytext);
							return T_DOUBLE; }

\"([^"\\\n]|{ES})*\"	{	size_t len = strlen(yytext) - 2;
							yylval.vstring = malloc(sizeof(char) * (len + 1)); 
							strncpy(yylval.vstring, yytext + 1, len);
							yylval.vstring[len] = '\0';
							return T_STRING; }

[:,\[\]\{\}]			{ return *yytext; }

{WS}+					{	/* whitespace separates tokens */ }

.						{	fprintf(stderr, "bad characters : %s", yytext); 
							return 1; }

%%

int yywrap(void)        /* called at end of input */
{
    return 1;           /* terminate now */
}

static void comment(void)
{
    int c;

    while ((c = input()) != 0)
        if (c == '*')
        {
            while ((c = input()) == '*')
                ;

            if (c == '/')
                return;

            if (c == 0)
                break;
        }
    yyerror("unterminated comment");
}


