#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdbool.h>

#define LEX_MAX_TOKEN_LEN 30
#define LEX_TOKENS_INIT_ALLOC 100
#define LEX_TOKENS_REALLOC_COEF 2

/*
Functions list :
lex_strtok : get all tokens
lex_free   : free memory after lex_strtok call
lex_strtot : string to type conversion
lex_ttostr : Type to string conversion
*/
/*
    Token types
*/
typedef enum token_type {
    /* basic types */
    T_TYPE_INT = 0, T_TYPE_DOUBLE, T_TYPE_CHAR, T_TYPE_VOID, 
	T_TYPE_INT_S, T_TYPE_DOUBLE_S, T_TYPE_CHAR_S, T_TYPE_VOID_S,

	T_STRUCT, T_TYPEDEF, T_ENUM,

    /* Specials */
    T_INCLUDE, T_DEFINE, T_RETURN,

	/* Logical */
	T_AND, T_OR, T_EXCL, T_IF, T_ELSE, T_ELSE_IF, 

    /* single chars */
    T_L_STRIPE, T_R_STRIPE, T_COMMA, T_SEMICOLON, T_EQUAL, T_SPACE, T_QUOTES,
    T_L_BRACKET, T_R_BRACKET, T_DOT, T_NEWLINE, T_EOS, T_TAB, T_HYPHEN, T_QUOTE,
	T_L_PAREN, T_R_PAREN, T_SHARP, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_AMPER,

	T_ESCAPE,

    /* End of tab index */
    T_EOT,


    /* Non-deterministic values, keep at end */
    T_LIT_INT, T_LIT_DOUBLE, T_LIT_STRING,
	T_VARNAME, //variable or function name
} lex_type;


typedef struct token {
    char literal[LEX_MAX_TOKEN_LEN];
    lex_type type;
} token;


/* Char is a digit */
bool char_is_int(char c);
/* Every char is a digit */
bool string_is_int(char *s);
bool string_is_double(char *s);
bool string_is_string(char *s);
/* Index is not the last */
bool not_EOT_literal(int i);
/* Index is not the last */
bool not_EOT_debug(int i);
/* Index is not the last */
bool not_EOT_EOW(int i);
/* Index is not the last */
bool not_EOT_ignore(int i);
/* Defines what an end of word char is */
bool char_is_EOW(char c);
/* Defines what an empty char is  */
bool char_is_ignore(char c);
/* Defines what an end of string char is  */
bool char_is_EOS(char c);
/* Defines what a valid char is  */
bool char_is_valid(char c);
/* Get type from string */
lex_type lex_strtot(char *expression);
/* Get literal from type */
const char *lex_ttostr(lex_type type);
/* Pointer to next non-valid char */
void pass_valid(char **p);
/* Pointer to next non-empty char */
void pass_ignore(char **p);
/* Advancing pointer till new type of word (valid, ignore, EOW)  */
void next_word(char **p);
/* Extracts tokens from string without empty chars */
token *lex_strtok(char *next);
/* Printing 1 token  */
void lex_print_t(token *t);
/* Printing all tokens from list */
void lex_print_l(token *l);
bool lex_is_t_basic(lex_type t);
/* Extracts n-1 element from list wich pointer is incremented by n.
 * (ignoring last token as it is a separator).
 * */
token *lex_extract(token **l, int n);
int lex_count(token *l, lex_type type);

