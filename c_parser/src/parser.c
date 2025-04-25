#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"


static const LEX_type p_directive[] = { L_DIRECTIVE, 0 };

static const LEX_type p_type_var[] = { L_TYPE, L_VARNAME, 0 }; // multiple types : static const int **var


pstruct *pstruct_new(P_STRUCT_TYPE t, LEX_token *start, LEX_token *end) {
	pstruct *res = malloc(sizeof(*res));
	res->type = t;
	res->len = end - start + 1;
	res->start = start;
	res->end = end;

	return res;
}

LEX_token *parser_get_funcname(pstruct *function) {
	for(LEX_token *i = function->start; i < function->end; i++) {
		if(i->type == L_VARNAME) {
			return i;
		}
	}
	return NULL;
}

LEX_token *pnext_non_blank(LEX_token ***tok) {
	while((*tok) && (**tok)->type == L_BLANK) {
		(*tok)++;
	}
	return **tok;
}

int pread(const LEX_type types[], LEX_token ***tok, LEX_token **first) {
	LEX_token ***tmp = tok;
	LEX_token *t = **tok;

	/* first non blank token */
	*first = NULL;

	for(size_t i = 0; types[i] != 0; i++) {
		// next non-blank
		t = pnext_non_blank(tmp);

		if(!*first) {
			*first = t;
		}

		if(t->type != types[i]) {
			return 0;
		}

		//TODO
		if(t->type == types[i]) {
			(*tmp)++; 
			t = **tmp;
		}

		while(types[i] == L_TYPE && t->type == L_TYPE) {
			(*tmp)++; 
			t = **tmp;
		}
	}

	*tok = *tmp;

	return 1;
}

pstruct *pread_directive(LEX_token ***tok) {
	pstruct *res = NULL;
	LEX_token *start;

	if(pread(p_directive, tok, &start)) {
		res = pstruct_new(P_DIRECTIVE, start, **tok);
#ifdef DEBUG
		printf("[DEBUG] %s : ", __func__);
		ltok_print(**tok);
#endif	
		(*tok)++;
	}
	return res;
}

pstruct *pread_var_decl(LEX_token ***tok) {
	pstruct *res = NULL;
	LEX_token *start;

	if((p_type_var, tok, &start)) {
		res = pstruct_new(P_VAR_DECL, start, **tok);
#ifdef DEBUG
		printf("[DEBUG] %s : ", __func__);
		ltok_print(**tok);
#endif	
		(*tok)++;
	}
	return res;
}

pstruct *pread_function_definition(LEX_token ***tok) {
	pstruct *res = NULL;
	LEX_token ***tmp = tok;
	LEX_token *t = **tok;


	if(t->type == L_TYPE) {
		t = pnext_non_blank(tmp);

		if(t->type == L_VARNAME) {
			t = pnext_non_blank(tmp);

			if(t->type == L_LPAREN) {
				t = pnext_non_blank(tmp);

			}
		}
	}

#ifdef DEBUG
	printf("[DEBUG] %s : ", __func__);
	ltok_print(t);
#endif

	return res;
}

void pignore_blanks(LEX_token ***tok) {
	while((**tok)->type == L_BLANK) {
		(*tok)++;
	}
}

arrayptr *parser(arrayptr *lextokens) {
	arrayptr *res = arrayptr_new(20);
	pstruct *s;

	LEX_token **t = (LEX_token **)(lextokens->items);
	while((char *) t < (char *) lextokens->items + lextokens->count) {
		s = NULL;
		pignore_blanks(&t);
		if(!s) { s = pread_directive(&t); }
		if(!s) { s = pread_function_definition(&t); }

		if(!s) {
			fprintf(stderr, "no structure found\n");
			return NULL;
		}
	}	

	return res;
}

