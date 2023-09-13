#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"


/* remove corresponding tokens from list */
list *parser_clean(list *l, unsigned int options) { //bool newlines, bool spaces, bool tabs) {
	assert(l);

	list *next = NULL;
	token *t = NULL;
	bool clean = false;
	bool first = true;

	for(list *p = l; p; p = next, clean = false) {
		next = p->next;
		t = p->data;
		assert(t);
		assert(t->literal);

		if(options & PC_COMMENT) {
			if(t->type == LT_COMMENT) {
				clean = true;
			}
		}
		if(options & PC_COMMENT_LINE) {
			if(t->type == LT_COMMENT && t->type2 == LT_COMMENT_LINE) {
				clean = true;
			}
		}
		if(options & PC_COMMENT_BLOCK) {
			if(t->type == LT_COMMENT && t->type2 == LT_COMMENT_BLOCK) {
				clean = true;
			}
		}
		if(options & PC_BLANK) {
			if(t->type == LT_BLANK) {
				clean = true;
			}
		}
		if(options & PC_NEWLINE) {
			if(t->type == LT_BLANK && t->type2 == LT_BLANK_NEWLINE) {
				clean = true;
			}
		}
		if(options & PC_TAB) {
			if(t->type == LT_BLANK && t->type2 == LT_BLANK_TAB) {
				clean = true;
			}
		}
		if(options & PC_SPACE) {
			if(t->type == LT_BLANK && t->type2 == LT_BLANK_SPACE) {
				clean = true;
			}
		}
		if(clean) {
			if(first) {
				l = l->next;
			}
			list_pop(p);
			list_free(p, lex_token_free);
		}
		else {
			first = false;
		}
	}

	return l;
}

list *parser_tokens_to_include(list *tokens) {
	return tokens;
}

list *parser_tokens_to_define(list *tokens) {
	return tokens;
}

list *parser_tokens_to_structs(list *tokens) {
	list *result = NULL;

	list *p = tokens;
	token *t0, *t1;

	t0 = p->data;

	if(t0->type == LT_OP_SHARP) {
		if(!p->next) {
			return NULL;
			fprintf(stderr, "parser_tokens_to_structs : error imissing token\n");
		}
		p = p->next;
		t1 = p->data;

		if(t1->type == LT_KW_INCLUDE) {
			p = p->next;
			result = parser_tokens_to_include(p);
			if(!result) {
				return NULL;
			}
		}
		else if(t1->type == LT_KW_DEFINE) {
			result = parser_tokens_to_define(p);
			if(!result) {
				return NULL;
			}
			else {
				fprintf(stderr, "parser_tokens_to_structs : error type %s\n", lex_type_lit(t1->type));
				return NULL;
			}
		}
	}

	return result;
}

/*
   token *parser_regroup_type(list *e0, int *n) {
   assert(e0);
   assert(e0->data);

   list *cur = e0;
   token *t0 = cur->data;

   if(parser_token_is_type(t0)) {

   cur = cur->next;
   if(!cur) { fprintf(stderr, "parser_regroup : missing token\n"); return NULL; }
   token *t1 = cur->data;

   if(parser_token_is_star(t1)) {

   cur = cur->next;
   if(!cur) { fprintf(stderr, "parser_regroup : missing token\n"); return NULL; }
   token *t2 = cur->data;

   if(parser_token_is_varname(t2)) {

   cur = cur->next;
   if(!cur) { fprintf(stderr, "parser_regroup : missing token\n"); return NULL; }
   token *t3 = cur->data;

   if(parser_is_end(t3)) {

   }

   if(parser_is_equal(t3)) {

   cur = cur->next;
   if(!cur) { fprintf(stderr, "parser_regroup : missing token\n"); return NULL; }
   token *t4 = cur->data;

   if(parser_is_value(t4)) {

   cur = cur->next;
   if(!cur) { fprintf(stderr, "parser_regroup : missing token\n"); return NULL; }
   token *t5 = cur->data;

   if(parser_is_end(t5)) {
   }
   }
   }
//function declation
if(parser_is_lparen(t3)) {

}
}
}
}

return NULL;
}

list *parser_specify(list *tokens) {
token *t = NULL;


return NULL;
}
*/

