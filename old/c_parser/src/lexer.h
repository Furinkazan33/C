#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"


#define LEX_TOK_INIT_LEN_ALLOC 16
#define LEX_TOK_ARRAY_ALLOC 128

typedef enum lex_token_type {
	LT_NONE = 0,
	LT_TYPE, 
	LT_VALUE, 
	LT_NAME, 
	LT_OPERATOR, 
	LT_KEYWORD,
	LT_BLANK,
	LT_COMMENT,
} lex_token_type;

typedef enum lex_operator {
	LT_OP_NONE = 0,
	LT_OP_DOT, 	
	LT_OP_EQUAL, LT_OP_PLUS, LT_OP_MINUS, LT_OP_DIV, LT_OP_STAR, LT_OP_PERCENT,
	LT_OP_AND,	LT_OP_OR,	LT_OP_NOT,	
	LT_OP_LHOOK,	LT_OP_RHOOK,
	LT_OP_LPAREN,	LT_OP_RPAREN,
	LT_OP_LSTRIPE,	LT_OP_RSTRIPE,
	LT_OP_LBRACKET,	LT_OP_RBRACKET,
	LT_OP_COLON,	LT_OP_SHARP,	LT_OP_COMMA,	LT_OP_SEMICOLON,
	LT_OP_QUOTES,	LT_OP_QUOTE,
} lex_operator;

typedef enum lex_value {
	LT_V_NONE = 0, 
	LT_V_INT, 
	LT_V_DOUBLE, 
	LT_V_CHAR, 
	LT_V_STRING,
} lex_value;

typedef enum lex_keyword {
	LT_KW_NONE = 0,
	LT_KW_CONST,
	LT_KW_TYPEDEF,
	LT_KW_DEFINE,
	LT_KW_INCLUDE,
	LT_KW_RETURN,
	LT_KW_IF,
	LT_KW_ELSE,
	LT_KW_WHILE,
	LT_KW_SWITCH,
	LT_KW_CASE,
	LT_KW_BREAK,
	LT_KW_DEFAULT,
	LT_KW_IFDEF,
	LT_KW_IFNDEF,
	LT_KW_ENDIF,
} lex_keyword;

typedef enum lex_blank {
	LT_BLANK_NONE = 0,
	LT_BLANK_NEWLINE,
	LT_BLANK_SPACE,
	LT_BLANK_TAB,
} lex_blank;

typedef enum lex_type {
	LT_T_NONE = 0,
	LT_T_VOID, 
	LT_T_INT, 
	LT_T_DOUBLE, 
	LT_T_CHAR, 
	LT_T_STRUCT,
	LT_T_ENUM,
} lex_type;

typedef enum lex_comment {
	LT_COMMENT_NONE = 0,
	LT_COMMENT_LINE,
	LT_COMMENT_BLOCK,
} lex_comment;

typedef struct token {
	char *literal;
	lex_token_type type;
	int type2;
} token;

token *lex_token_new(char *literal, lex_token_type type, int type2);
void lex_token_write(void *token);
void lex_token_free(void *token);

/* set token types from literal value */
token *lex_token_set_types(token *t);

/* return literal value from type */
char *lex_LTT_literal(lex_token_type t);
char *lex_LO_literal(lex_operator t);
char *lex_LV_literal(lex_value t);
char *lex_LKW_literal(lex_keyword t);
char *lex_LB_literal(lex_blank t);
char *lex_LT_literal(lex_type t);
char *lex_LC_literal(lex_comment t);

/* convert code to list of strings */
list *lex_code_to_strings(char *code, int *n);

/* convert list of strings to list of tokens */
list *lex_strings_to_tokens(list *strings);

