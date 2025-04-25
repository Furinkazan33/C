#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "arrayptr.h"


typedef enum {
	L_NONE = 0, 
	L_DIRECTIVE, 
	L_TYPE,
	L_RESERVED, 
	L_COMMENT, 
	L_COMMENTS,
	L_VARNAME,
	L_ARROW,
	L_BLANK,
	L_LPAREN, 
	L_RPAREN, 
	L_LBRACKET, 
	L_RBRACKET, 
	L_LHOOK, 
	L_RHOOK, 
	L_QUOTE, 
	L_DQUOTE, 
	L_EQUAL, 
	L_COMMA, 
	L_COLON, 
	L_SEMI, 
	L_DOT, 
	L_SPACE, 
	L_BSn, 
	L_BSt, 
	L_LIT_INT,
} LEX_type;

typedef struct LEX_token {
	LEX_type type;
	char *start;
	char *end;
	size_t len;
} LEX_token;

void ltok_print(LEX_token *t);

arrayptr *lexer(char *text);

