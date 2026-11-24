#include <stdbool.h>
#include <stdio.h>
#include "array.h"


/*
	TODO: add escape character
*/

typedef enum {
	TNZ_T1_NONE = 0,
} tnz_type_1;

typedef enum { 
	TNZ_T2_NONE = 0,
} tnz_type_2;

typedef struct t_block {
	const char *start;
	const char *end;
	int type1;
	int type2;
} t_block;

void t_block_print(const t_block *b);

typedef struct t_reserved {
	const char *content;
	int type1;
	int type2;
} t_reserved;

void t_reserved_print(const t_reserved *r);

typedef struct token {
	int type1;
	int type2;
	char *content;
} token;


void token_set(token *t, int type1, int type2, char *content, size_t len);
void token_free(void *tok);
void tokens_free(array *t);

extern const t_block blocks[];
extern const t_reserved reserved[];

array *tokenize(array **dest, char *text, const t_block blocks[], const t_reserved reserved[]);

void token_fprint(void *t, FILE *out);
void token_print(void *tok);
void token_fprint_formated(void *tok, FILE *out);
void token_print_formated(void *tok);

void tokens_print(array *tokens);
void tokens_fprint(array *tokens, FILE *out);
void tokens_print_formated(array *tokens);
void tokens_fprint_formated(array *tokens, FILE *out);


char *readfile(const char *filename);


bool char_is_digit(char c);
bool char_is_alpha(char c);
bool char_is_alnum(char c);
bool char_is_varname_first(char c);
bool char_is_varname(char c);

bool string_is_integer(char *s, size_t len);
bool string_is_alpha(char *s, size_t len);
bool string_is_alnum(char *s, size_t len);
bool string_is_varname(char *s, size_t len);

