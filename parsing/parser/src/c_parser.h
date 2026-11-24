#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "tokenizer.h"



/*
	Parser base structure.
*/
typedef enum {
	P_SIMPLE_STATEMENT,
	P_BLOCK,
	P_FUNC
} p_struct_type;

typedef struct p_struct {
	p_struct_type type;
	token *start;
	token *end;
} p_struct;

p_struct *p_struct_new(p_struct_type type, token *start, token *end);
void p_struct_free(void *s);

void parser_fprint(void *p, FILE *out);
void parser_fprint_formated(void *p, FILE *out);

void parser_print(void *p);
void parser_print_formated(void *p);

array *parse(array **dest, array *tokens);


