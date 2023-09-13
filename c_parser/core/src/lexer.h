#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"


#define LEX_TOK_INIT_LEN_ALLOC 16
#define LEX_TOK_ARRAY_ALLOC 128

typedef enum lex_type {
	LT_NONE = 0,
	LT_OPERATOR, 
		LT_OP_DOT,
		LT_OP_EQUAL,
		LT_OP_PLUS,
		LT_OP_MINUS,
		LT_OP_DIV,
		LT_OP_STAR,
		LT_OP_DIVE,
		LT_OP_PERCENT,
		LT_OP_AND,
		LT_OP_OR,
		LT_OP_NOT,
		LT_OP_LHOOK,
		LT_OP_RHOOK,
		LT_OP_LPAREN,
		LT_OP_RPAREN,
		LT_OP_LSTRIPE,
		LT_OP_RSTRIPE,
		LT_OP_LBRACKET,
		LT_OP_RBRACKET,
		LT_OP_COLON,
		LT_OP_SHARP,
		LT_OP_COMMA,
		LT_OP_SEMICOLON,
	LT_TYPE, 
		LT_T_VOID, 
		LT_T_INT, 
		LT_T_DOUBLE, 
		LT_T_CHAR, 
		LT_T_STRUCT,
		LT_T_ENUM,
	LT_VALUE, 
		LT_V_INT, 
		LT_V_DOUBLE, 
		LT_V_CHAR, 
		LT_V_STRING,
	LT_KEYWORD,
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
		LT_KW_ENDIF,
	LT_COMMENT,
		LT_COMMENT_LINE,
		LT_COMMENT_BLOCK,
	LT_BLANK,
		LT_BLANK_NEWLINE,
		LT_BLANK_SPACE,
		LT_BLANK_TAB,
	LT_NAME, 
} lex_type;

typedef struct token {
    char *literal;
    lex_type type;
    lex_type type2;
} token;

token *lex_tok_new(char *literal, lex_type type);
void lex_string_write(void *s, void *f);
void lex_token_write(void *token, void *file);
void lex_token_free(void *token);

/* set token types from literal value */
token *lex_token_set_types(token *t);

/* return literal value from type */
char *lex_type_lit(lex_type t);

/* convert code to list of strings */
list *lex_code_to_strings(char *code, int *n);

/* convert list of strings to list of tokens */
list *lex_strings_to_tokens(list *strings);

