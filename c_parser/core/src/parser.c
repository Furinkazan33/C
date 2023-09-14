#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"


char *parser_type_literal(c_stat_type type) {
	switch(type) {
		case CST_NONE:
			return "NONE";
		case CST_INCLUDE: 
			return "INCLUDE";
		case CST_DEFINE: 
			return "DEFINE";
		case CST_VAR_DECL: 
			return "VAR_DECL";
		case CST_VAR_ASSIGN: 
			return "VAR_ASSIGN";
		case CST_VAR_DECL_ASSIGN: 
			return "VAR_DECL_ASSIGN";
		case CST_FUNC_DECL: 
			return "FUNC_DECL";
		case CST_FUNC_DECL_FULL: 
			return "FUNC_DECL_FULL";
		case CST_FUNC_CALL: 
			return "FUNC_CALL";
		case CST_ARRAY: 
			return "ARRAY";
		case CST_RETURN: 
			return "RETURN";
		case CST_IF: 
			return "IF";
		case CST_ELSE: 
			return "ELSE";
		case CST_SWITCH: 
			return "SWITCH";
		case CST_CASE: 
			return "CASE";
		case CST_DEFAULT: 
			return "DEFAULT";
		case CST_LOGICAL: 
			return "LOGICAL";
		default:
			fprintf(stderr, "parser_type_literal : unknown type : %d\n", type);
			return NULL;
	}
}

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

bool _is_include(list *l) {
	token *t = l->data;

	if(t->type == LT_OPERATOR && t->type2 == LT_OP_SHARP) {
		if(!l->next) {
			return NULL;
			fprintf(stderr, "parser_is_include : missing token after reading [%s]\n", t->literal);
		}
		l = l->next;
		t = l->data;

		if(t->type == LT_KEYWORD && t->type2 == LT_KW_INCLUDE) {
			return true;
		}
	}
	return false;
}

//TODO
c_stat_type parser_get_type(list *l) {
	if(_is_include(l)) {
		return CST_INCLUDE;
	}

	return CST_NONE;
}

c_include *parser_new_include(bool local, char *filepath) {
	c_include *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_include : call to malloc returned NULL\n");
		return NULL;
	}
	res->local = local;
	res->filepath = filepath;

	return res;
}

c_include *parser_read_include(list **p0) {
	assert(*p0);

	token *t0, *t1, *t2;
	list *p1, *p2;
	char *filepath = NULL;
	c_include *res = NULL;
	bool local;

	/* checking "#" */
	t0 = (*p0)->data;
	if(t0->type != LT_OPERATOR || t0->type2 != LT_OP_SHARP) {
		fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t0->type, t0->literal);
		return NULL;
	}
	*p0 = (*p0)->next;
	if(!*p0) {
		fprintf(stderr, "parser_read_include : missing token\n");
		return NULL;
	}

	/* checking "include" */
	t0 = (*p0)->data;
	if(t0->type != LT_KEYWORD || t0->type2 != LT_KW_INCLUDE) {
		fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t0->type, t0->literal);
		return NULL;
	}
	*p0 = (*p0)->next;
	if(!*p0) {
		fprintf(stderr, "parser_read_include : missing token\n");
		return NULL;
	}

	/* checking "<" or string value for local include */
	t0 = (*p0)->data;
	if(!(t0->type == LT_OPERATOR && t0->type2 == LT_OP_LSTRIPE) && !(t0->type == LT_VALUE && t0->type2 == LT_V_STRING)) {
		fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t0->type, t0->literal);
		return NULL;
	}

	/* local include 
	 * p0 : filepath as LT_V_STRING */
	if(t0->type == LT_VALUE && t0->type2 == LT_V_STRING) {
		local = true;

		filepath = malloc(sizeof(char) * (strlen(t0->literal) + 1));
		if(!filepath) {
			fprintf(stderr, "parser_read_include : call to malloc(0) returned NULL\n");
			return NULL;
		}
		strcpy(filepath, t0->literal);

		*p0 = (*p0)->next;
	}
	/* p0 : filepath as LT_NAME, p1 : '.', p2 : file extension as LT_NAME
	 * or
	 * p0 : filepath, p1 : '>' */
	else {
		local = false;
		*p0 = (*p0)->next;
		if(!*p0) {
			fprintf(stderr, "parser_read_include : missing token\n");
			return NULL;
		}
		t0 = (*p0)->data;

		/* checking filepath*/
		if(t0->type != LT_NAME) {
			fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t0->type, t0->literal);
			return NULL;
		}
		p1 = (*p0)->next;
		if(!p1) {
			fprintf(stderr, "parser_read_include : missing token\n");
			return NULL;
		}
		t1 = p1->data;

		/* checking file extension '.' or end of include '>' */
		if(t1->type != LT_OPERATOR) {
			fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t1->type, t1->literal);
			return NULL;
		}

		/* filepath contains a '.' */
		if(t1->type2 == LT_OP_DOT) {
			p2 = p1->next;
			if(!p2) {
				fprintf(stderr, "parser_read_include : missing token\n");
				return NULL;
			}
			t2 = p2->data;

			/* checking file extension */
			if(t2->type != LT_NAME) {
				fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t2->type, t2->literal);
				return NULL;
			}

			filepath = malloc(sizeof(char) * (strlen(t0->literal) + strlen(t1->literal) + strlen(t2->literal) + 1));
			if(!filepath) {
				fprintf(stderr, "parser_read_include : call to malloc(1) returned NULL\n");
				return NULL;
			}
			strcpy(filepath, t0->literal); // filepath
			strcat(filepath, t1->literal); // dot
			strcat(filepath, t2->literal); // extension
			filepath[strlen(filepath)] = '\0';

			*p0 = p2->next;
		}
		/* no file extension in path */
		else {
			if(t1->type2 != LT_OP_RSTRIPE) {
				fprintf(stderr, "parser_read_include : statement format error : [%d][%s]\n", t1->type, t1->literal);
				return NULL;
			}

			filepath = malloc(sizeof(char) * (strlen(t0->literal) + 1));
			if(!filepath) {
				fprintf(stderr, "parser_read_include : call to malloc(2) returned NULL\n");
				return NULL;
			}
			strcpy(filepath, t0->literal);
			filepath[strlen(filepath)] = '\0';

			*p0 = (*p0)->next;
		}
	}

	res = parser_new_include(local, filepath);
	if(!res) {
		fprintf(stderr, "parser_read_include : call to parser_new_include returned NULL\n");
		return NULL;
	}

	return res;
}

list *parser_tokens_to_structs(list *tokens) {
	list *res = NULL;
	list *p = tokens;
	c_statement *s;
	c_stat_type type;

	while(p) {
		type = parser_get_type(p);

		if(type == CST_NONE) {
			fprintf(stderr, "parser_tokens_to_structs : call to parser_get_type returned CST_NONE\n");
			return NULL;
		}

		s = malloc(sizeof(c_statement));
		if(!s) {
			fprintf(stderr, "parser_tokens_to_structs : call to malloc returned NULL\n");
			return NULL;
		}
		s->type = type;

#ifdef DEBUG
		fprintf(stdout, "parser_tokens_to_structs : [%s] => [%d]\n", ((token *)p->data)->literal, type);
#endif
		switch(type) {
			case CST_INCLUDE:
				s->data = parser_read_include(&p);
				break;

				//TODO:
				
			default:
				fprintf(stderr, "parser_tokens_to_structs : unknown statement : %d\n", type);
				return NULL;
		}

		if(!s->data) {
			fprintf(stderr, "parser_tokens_to_structs : error creating new statement\n");
			return NULL;
		}
		if(!res) {
			res = list_new(s);
		}
		else {
			res = list_insert_after(res, s);
		}
		if(!res) {
			fprintf(stderr, "parser_tokens_to_structs : error adding c statement\n");
			return NULL;
		}
		p = p->next;
	}

	return res;
}

void parser_print_statement(void *s, void *f) {
	c_statement *stat = s;
	c_stat_type type = stat->type;
	c_include *inc = NULL;

	switch(type) {
		case CST_INCLUDE:
			inc = stat->data;
			fprintf((FILE *)f, "statement : include [%d][%s]\n", inc->local, inc->filepath);
			break;
		default:
			fprintf((FILE *)f, "statement : default\n");
			break;
	}
}


