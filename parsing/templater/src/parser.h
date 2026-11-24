#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "tokenizer.h"


typedef enum {
	P_NONE,
	P_WS, 
	P_VARIABLE,
} p_struct_type;

typedef struct p_struct {
	p_struct_type type;
	token *start;
	token *end;
} p_struct;

void p_struct_set(p_struct *dest, p_struct_type type, token *start, token *end);

void pstructs_free(array *p);

void pstruct_fprint(void *p, FILE *out);
void pstruct_fprint_formated(void *p, FILE *out);
void pstruct_print(void *p);
void pstruct_print_formated(void *p);

void pstructs_fprint(array *a, FILE *out);
void pstructs_fprint_formated(array *a, FILE *out);
void pstructs_print(array *a);
void pstructs_print_formated(array *a);

array *parse_param(array *dest, array *tokens);
array *parse_template(array *dest, array *tokens);
array *parse_replace(array *params, array *tpl);

// main function
int parse_and_replace(char *filepath_params, char *filepath_tpl, char *filepath_result);


