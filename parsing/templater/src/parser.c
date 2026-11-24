#include <string.h>
#include "parser.h"
#include "tnz_param.h"
#include "tnz_tpl.h"
#include "macros.h"



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

void pstructs_free(array *a) {
	ARRAY_FREE(a);
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

void pstruct_fprint(void *p, FILE *out){
	fprintf(out, "%s", p_struct_as_string((p_struct *) p));
}

void pstruct_fprint_formated(void *p, FILE *out){
	fprintf(out, "[%s]", p_struct_as_string((p_struct *) p));
}

void pstruct_print(void *p){
	printf("%s", p_struct_as_string((p_struct *) p));
}

void pstruct_print_formated(void *p){
	printf("[%s]", p_struct_as_string((p_struct *) p));
}


void pstructs_fprint(array *a, FILE *out){
	ARRAY_DO(a, idx, void *, p, pstruct_fprint(p, out); );
}

void pstructs_fprint_formated(array *a, FILE *out){
	ARRAY_DO(a, idx, void *, p, pstruct_fprint_formated(p, out); );
}

void pstructs_print(array *a){
	ARRAY_DO(a, idx, void *, p, pstruct_print(p); );
}

void pstructs_print_formated(array *a){
	ARRAY_DO(a, idx, void *, p, pstruct_print_formated(p); );
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

#define READ_VALID_OR_NULL(valid, last, pos) { \
	if(pos == last || !valid(pos)) { return NULL; } \
	pos++; \
}

#define IGNORE_TYPE1(type, last, pos) { \
	while(pos != last && pos->type1 == type) { \
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
	READ_OR_NULL(type1, WS, last, end);
	
	p_struct_set(dest, P_WS, *start, end);
	
	#ifdef DEBUG_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

p_struct *read_template_any(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	
	while(end != last && (end->type1 == WS || end->type1 == TNZ_T1_NONE)) {
		end++;
	}
	
	p_struct_set(dest, P_NONE, *start, end);
	
	#ifdef DEBUG_PARSER
		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}

// inside template
p_struct *read_variable(p_struct *dest, token *last, token **start) {
	token *end = *start;
		
	READ_OR_NULL(type1, VAR_START, last, end); 
	READ_OR_NULL(type1, TNZ_T1_NONE, last, end); 
	READ_OR_NULL(type1, VAR_END, last, end); 
	
	p_struct_set(dest, P_VARIABLE, *start, end);
	
	#ifdef DEBUG_PARSER
		LOG_S("");
		printf(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}


// inside param
p_struct *read_param(p_struct *dest, token *last, token **start) {
	token *end = *start;
	
	READ_OR_NULL(type1, TNZ_T1_NONE, last, end);  IGNORE_TYPE1(WS, last, end);
	READ_OR_NULL(type1, EQUAL, last, end);  IGNORE_TYPE1(WS, last, end);
	
	
	if(end != last && (end->type1 == TNZ_T1_NONE || end->type1 == VALUE)) { 	end++; }
	else { return NULL; }
	
	IGNORE_TYPE1(WS, last, end);
	
	READ_OR_NULL(type1, SEMI, last, end);
	
	p_struct_set(dest, P_VARIABLE, *start, end);
	
	#ifdef DEBUG_PARSER

		LOG_S(" yes\n");
	#endif
	
	*start = end;
	
	return dest;
}


// get token containing value
token *param_get_value(p_struct *param) {
	token *p = param->start + 1;
	
	while(p != param->end) {
		if(p->type1 == TNZ_T1_NONE || p->type1 == VALUE) {
			return p;
		}
		p++;
	}
	
	return NULL;
}

// get param of variable
p_struct *param_get(array *params, char *search) {
	p_struct *p = ARRAY_GET(params, 0, p_struct *);
	p_struct *last = ARRAY_GET(params, params->count, p_struct *);
	
	while(p != last) {
		// p->start token contains variable name
		if(!strcmp(p->start->content, search)) {
			return p;
		}
		p++;
	}
	
	return NULL;
}


array *parse_param(array *dest, array *tokens) {
	if(!tokens || tokens->count == 0) {
		LOG_E("empty array\n");
		return NULL;
	}
	
	token *p = ARRAY_GET(tokens, 0, token *);
	p_struct src;
	
	token *last = ARRAY_GET(tokens, tokens->count, token *);
	
	while(p != last) {
		#ifdef DEBUG_PARSER
			LOG_S("p=");
			token_print_formated(p);
			puts("");
		#endif
		
		p_struct *s = read_ws(&src, last, &p);
		if(!s) { s = read_param(&src, last, &p); }
		
		if(!s) {
			LOG_E("no match at :");
			token_fprint(p, stderr);
			fprintf(stderr, "\n");
			return NULL;
		}
		else {
			p_struct *new = p_struct_cpy(&src); if(!new) { LOG_E("error from p_struct_cpy"); return NULL; }
			array_add(dest, new);
		}
	}
	
	return dest;
}

array *parse_template(array *dest, array *tokens) {
	if(!tokens || tokens->count == 0) {
		LOG_E("empty array\n");
		return NULL;
	}
	
	token *p = ARRAY_GET(tokens, 0, token *);
	p_struct src;
	
	token *last = ARRAY_GET(tokens, tokens->count, token *);
	
	while(p != last) {
		#ifdef DEBUG_PARSER
			LOG_S("p=");
			token_print_formated(p);
			puts("");
		#endif
		
		p_struct *s = read_ws(&src, last, &p);
		if(!s) { s = read_variable(&src, last, &p); }
		if(!s) { s = read_template_any(&src, last, &p); }
		
		if(!s) {
			LOG_E("no match at :");
			token_fprint(p, stderr);
			fprintf(stderr, "\n");
			return NULL;
		}
		else {
			p_struct *new = p_struct_cpy(&src); if(!new) { LOG_E("error from p_struct_cpy"); return NULL; }
			array_add(dest, new);
		}
	}
	
	return dest;
}


array *parse_replace(array *params, array *tpl) {
	if(!params || params->count == 0) {
		LOG_E("empty params\n");
		return NULL;
	}
	
	if(!tpl || tpl->count == 0) {
		LOG_E("empty tpl\n");
		return NULL;
	}
	
	p_struct *p_tpl = ARRAY_GET(tpl, 0, p_struct *);
	p_struct *last_tpl = ARRAY_GET(tpl, tpl->count, p_struct *);
	
	while(p_tpl != last_tpl) {
		
		
		// variable found => replace
		if(p_tpl->type == P_VARIABLE) {
			#ifdef DEBUG_REPLACE
				LOG_S("p=");
				parser_print_formated(p_tpl);
				puts("");
			#endif
		
			p_tpl->start = p_tpl->start + 1;
			p_tpl->end = p_tpl->start + 1;
			
			// searching for value in params
			p_struct *param = param_get(params, p_tpl->start->content);
			if(!param) {
				LOG_E("param not found");
				return NULL;
			}
			token *var = param_get_value(param);
			if(!var) {
				LOG_E("value not found");
				return NULL;
			}
			
			// replacing
			p_tpl->start->content = realloc(p_tpl->start->content, sizeof(char) * (strlen(var->content) + 1));
			strcpy(p_tpl->start->content, var->content);
			
			
			#ifdef DEBUG_REPLACE
				LOG_S("replace=");
				printf("[%s]\n", p_tpl->start->content);
				puts("");
			#endif
		}
	
		p_tpl++;
	}
	
	return tpl;
}



