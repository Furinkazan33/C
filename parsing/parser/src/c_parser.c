#include <string.h>
#include "c_parser.h"
#include "tnz_c.h"
#include "macros.h"


p_struct *p_struct_new(p_struct_type type, token *start, token *end) {
	p_struct *res = malloc(sizeof(*res));
	if(!res) {
		perror("");
		LOG_E("malloc failed");
		return NULL;
	}
	res->type = type;
	res->start = start;
	res->end = end;
	return res;
}

p_struct *p_struct_cpy(p_struct *src) {
	p_struct *res = malloc(sizeof(*res));
	if(!res) {
		LOG_E("malloc failed");
		perror("");
		return NULL;
	}
	res->type = src->type;
	res->start = src->start;
	res->end = src->end;
	return res;
}

void p_struct_set(p_struct *dest, p_struct_type type, token *start, token *end) {
	dest->type = type;
	dest->start = start;
	dest->end = end;
}

void p_struct_free(void *s) {
	free(s);
}

char *p_struct_as_string(p_struct *s) {
	size_t alloc = 20;
	char *res = malloc(sizeof(char) * (alloc + 1));
	if(!res) {
		LOG_E("malloc failed");
		perror("");
		return NULL;
	}
	res[0] = 0;
	
	for(token *p = s->start; p < s->end; p++) {
		while(strlen(res) + strlen(p->content) > alloc) {
			res = realloc(res, sizeof(char) * (alloc * 2 + 1));
			if(!res) {
				LOG_E("realloc failed");
				perror("");
				return NULL;
			}
			alloc *= 2;
		}
		strcat(res, p->content);
	}
	
	return res;
}

void parser_fprint(void *p, FILE *out){
	fprintf(out, "%s", p_struct_as_string((p_struct *) p));
}

void parser_fprint_formated(void *p, FILE *out){
	fprintf(out, "[%s]", p_struct_as_string((p_struct *) p));
}

void parser_print(void *p){
	printf("%s", p_struct_as_string((p_struct *) p));
}

void parser_print_formated(void *p){
	printf("[%s]", p_struct_as_string((p_struct *) p));
}


bool token_is_varname(token *t) {
	char *s = t->content;
	
	if(!char_is_varname_first(*s)) {
		return false;
	}
	else {
		for(size_t i = 1; s[i]; i++) {
			if(!char_is_varname(s[i])) {
				return false;
			}
		}
		return true;
	}
}

#define READ_OR_NULL(type, value, last, pos) { \
	if(pos == last || pos->type != value) { return NULL; } \
	pos++; \
}

#define READ_S_OR_NULL(type, value, last, pos) { \
	if(pos == last || pos->type != value) { return NULL; } \
	while(pos != last && pos->type == value) { \
		pos++; \
	} \
}

#define READ_VALID_OR_NULL(valid, last, pos) { \
	if(pos == last || !valid(pos)) { return NULL; } \
	pos++; \
}

#define IGNORE_C_LEX_WS(last, pos) { \
	while(pos != last && pos->type1 == C_LEX_WS) { \
		pos++; \
	} \
}

#define IGNORE_UNTIL(type, what, last, pos) { \
	while(pos != last && pos->type != what) { \
		pos++; \
	} \
}

p_struct *read_ws(p_struct *dest, token *last, token **start) {
	token *end = *start;	
	READ_OR_NULL(type1, C_LEX_WS, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

// multiple ws
p_struct *read_ws_s(p_struct *dest, token *last, token **start) {
	token *end = *start;	
	READ_S_OR_NULL(type1, C_LEX_WS, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

p_struct *read_preproc(p_struct *dest, token *last, token **start) {
	token *end = *start;	
	READ_OR_NULL(type1, C_LEX_PREPROC, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

p_struct *read_vardecl(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	/* reading types */
	if(end != last && end->type1 == C_LEX_TYPE) { 
		while(end->type1 == C_LEX_TYPE || end->type1 == C_LEX_WS) { 	end++; }
	}
	else 						{ return NULL; }
	
	/* reading stars */
	while(end != last && end->type2 == C_LEX_STAR) { end++; }
	IGNORE_C_LEX_WS(last, end);
	
	READ_VALID_OR_NULL(token_is_varname, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_SEMI, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}


// TODO: read until SEMI
p_struct *read_varassign(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	/* reading stars */
	while(end != last && end->type2 == C_LEX_STAR) { end++; }
	IGNORE_C_LEX_WS(last, end);
	
	READ_VALID_OR_NULL(token_is_varname, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_EQUAL, last, end);  IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type1, TNZ_T1_NONE, last, end);  IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_SEMI, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

// TODO: read until SEMI
p_struct *read_vardeclassign(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	/* reading types */
	if(end != last && end->type1 == C_LEX_TYPE) { 
		while(end != last && (end->type1 == C_LEX_TYPE || end->type1 == C_LEX_WS)) 
											{ 	end++; }
	}
	else 									{ return NULL; }
	
	/* reading stars */
	while(end != last && end->type2 == C_LEX_STAR) { end++; }
	IGNORE_C_LEX_WS(last, end);
		
	READ_VALID_OR_NULL(token_is_varname, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_EQUAL, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type1, TNZ_T1_NONE, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_SEMI, last, end);
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

p_struct *read_function(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	/* reading types */
	if(end != last && end->type1 == C_LEX_TYPE) { 
		while(end != last && (end->type1 == C_LEX_TYPE || end->type1 == C_LEX_WS)) 
											{ 	end++; }
	}
	else 									{ return NULL; }
	
	READ_VALID_OR_NULL(token_is_varname, last, end); IGNORE_C_LEX_WS(last, end);
	READ_OR_NULL(type2, C_LEX_LPAREN, last, end); IGNORE_C_LEX_WS(last, end);
	
	/* missing reading params */
	
	IGNORE_UNTIL(type2, C_LEX_RPAREN, last, end);
	READ_OR_NULL(type2, C_LEX_RPAREN, last, end); IGNORE_C_LEX_WS(last, end);
	
	READ_OR_NULL(type2, C_LEX_LBRACK, last, end); IGNORE_C_LEX_WS(last, end);
	
	/* missing reading block */
	
	IGNORE_UNTIL(type2, C_LEX_RBRACK, last, end);
	READ_OR_NULL(type2, C_LEX_RBRACK, last, end); 
	
	
	p_struct_set(dest, P_SIMPLE_STATEMENT, *start, end);
	
	#ifdef DEBUG_C_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

p_struct *read_any(p_struct *dest, token *last, token **start) {
	p_struct *s = read_ws(dest, last, start);
	if(!s) { s = read_preproc(dest, last, start); }
	if(!s) { s = read_vardecl(dest, last, start); }
	if(!s) { s = read_varassign(dest, last, start); }
	if(!s) { s = read_vardeclassign(dest, last, start); }
	if(!s) { s = read_function(dest, last, start); }
	
	return s;
}


array *parse(array **dest, array *tokens) {
	if(!tokens || tokens->count == 0) {
		LOG_E("empty array\n");
		return NULL;
	}
	
	token *p = ARRAY_GET(tokens, 0, token *);
	p_struct src;
	
	token *last = ARRAY_GET(tokens, tokens->count, token *);
	
	while(p != last) {
		#ifdef DEBUG_C_PARSER
			LOG_S("current token=");
			token_print_formated(p);
			puts("");
		#endif
		
		p_struct *s = read_any(&src, last, &p);

		if(!s) {
			LOG_E("no match starting at :");
			token_fprint(p, stderr);
			fprintf(stderr, "\n");
			return NULL;
		}
		else {
			p_struct *new = p_struct_cpy(&src); if(!new) { LOG_E("error from p_struct_cpy"); return NULL; }
			array_add(*dest, new);
		}
	}
	
	return *dest;
}



