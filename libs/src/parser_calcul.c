#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include "parser_calcul.h"
#include "macros.h"
#include "arrayh.h"

#define MAX_TOK_LEN 256

typedef float (fun_op)(float, float);


expr *expr_alloc() {
	expr *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->type = 0;	
	res->value = 0;
	res->op = 0;
	res->left = NULL;
	res->right = NULL;
	
	return res;
}

expr *expr_new(expr_type type, float value, char op, expr *left, expr *right) {
	expr *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	res->type = type;
	
	if(type == ET_VALUE) {
		res->value = value;
		res->op = 0;
		res->left = NULL;
		res->right = NULL;
	}
	else {
		res->value = 0;
		res->op = op;
		res->left = left;
		res->right = right;
	}
	
	return res;
}

void expr_free(expr *e) {
	if(!e) return;
	expr_free(e->left);
	expr_free(e->right);
	free(e);
}



bool isoperator(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/'; 
}

bool isoperator1(char c) {
	return c == '*' || c == '/'; 
}

bool isoperator2(char c) {
	return c == '+' || c == '-'; 
}

bool isparen(char c) {
	return c == '(' || c == ')'; 
}

bool read_string(char *start, char **end, char *string) {
	*end = start;
	
	while(*end && **end && string && *string && **end == *string) {
		(*end)++;
		string++;
	}

	return *string == '\0';
}

bool readn_string(char *start, char **end, char *string, size_t len) {
	*end = start;
	
	for(size_t i = 0; i < len; i++) {
		if(**end != string[i]) {
			return false;
		}
		(*end)++;
	}
	
	return true;
}

bool read_digits(char *start, char **end) {
	*end = start;
	
	while(isdigit(**end)) {
		(*end)++;
	}

	return (*end) > start;
}

bool read_atomic_float(char *start, char **end) {
	*end = start;
	bool res = false;
	
	if(read_digits(*end, end)) {
		res = true;
	}
	if(read_string(*end, end, ".")) {
		if(read_digits(*end, end)) {
			res = true;
		}
	}
	
	return res;
}

bool read_operator(char *start, char **end) {
	*end = start;
	
	if(isoperator(**end)) {
		(*end)++;
		return true;
	}
	else {
		return false;
	}
}

bool read_spaces(char *start, char **end) {
	*end = start;
	
	if(!isblank(**end)) {
		return false;
	}
	
	while(**end && isblank(**end)) {
		(*end)++;
	}
	
	return true;
}

bool read_any(char *start, char **end) {
	*end = start;
	
	if(isoperator(**end) || read_string(start, end, "(") || read_string(start, end, ")")) {
		(*end)++;
		return true;
	}
	else if(read_atomic_float(start, end)) {
		return true;
	}
	else if(read_spaces(start, end)) {
		return true;
	}
	else {
		return false;
	}
}



void ignore_spaces(char **src) {
	while(**src && isspace(**src)) {
		(*src)++;
	}
}

void ignore_spaces_b(char *start, char **src) {
	while(**src != start && **src && isspace(**src)) {
		(*src)--;
	}
}


bool to_next(char **src) {
	char *end;
	
	if(read_spaces(*src, &end)) {
		read_any(end, &end);
	}
	else {
		read_any(*src, &end);
		read_spaces(end, &end);
	}
	
	if(end && end != *src) {
		*src = end;
		return true;
	}
	else {
		return false;
	}
}

// TODO
bool to_previous(char *bos, char **src) {
	char *end;
	
	if(read_spaces_b(*src, &end)) {
		read_any_b(end, &end);
	}
	else {
		read_any_b(*src, &end);
		read_spaces_b(end, &end);
	}
	
	if(end && end != *src) {
		*src = end;
		return true;
	}
	else {
		return false;
	}
}

char *next(int n, char *src) {
	size_t i;
	for(i = 0; i < n && to_next(&src); i++) {
		;
	}
	
	if(i == n) {
		return src;
	}
	else {
		return NULL;
	}
}

char *previous(int n, char *bos, char *src) {
	size_t i;
	for(size_t i = 0; i < n && to_previous(bos, &src); i++) {
		;
	}
	
	if(i == n) {
		return src;
	}
	else {
		return NULL;
	}
}

char *get_current(char *dest, size_t alloc_len, char *src) {
	char *end;
	
	if(read_operator(src, &end) || read_string(src, &end, "(") || read_string(src, &end, ")")) {
		dest[0] = src[0];
		dest[1] = 0;
	}
	else {
		read_atomic_float(src, &end);
		
		if(end - src >= alloc_len) {
			LOG_E_MESS("word too long !\n");
			return NULL;
		}
		else {
			strcpy(dest, src, end - src);
			dest[end - src] = 0;
		}
	}
	
	return dest;
}

char *get(int n, char *dest, size_t alloc_len, char *src) {
	char *p;
	
	if(n >= 0) {
		p = next(n, src);
		if(!p) {
			LOG_E_MESS("no such word !\n");
			return NULL;
		}
	}
	else {
		p = previous(-n, src)
		if(!p) {
			LOG_E_MESS("no such word !\n");
			return NULL;
		}
	}
	
	if(!get_current(dest, alloc_len, src)) {
		return NULL;
	}
	
	return dest;
}





expr *strtoe(char **s, expr *left) {
	char buf[64];
	char *end;
	char *p = *s;

	while(*p) {
		ignore_spaces(&p);
		
		// right value
		if(left) {
			if(read_atomic_float(p, &end)) {
				if(!get_current(buf, 64, p)) return NULL;
				expr *res = expr_new(ET_VALUE, atof(buf), 0, NULL, NULL); if(!res) return NULL;
			}
			else if(read_string(p, &end, "(")) {
				//TODO
				
			}
			else {
				LOG_E("malformed expression after [%s]\n", p);
				return NULL;
			}
			
			p = next(1, p);
			*s = p;
			return res;
		}
		// left value
		else {
			if(read_atomic_float(p, &end)) {
				if(!get_current(buf, 64, p)) return NULL;
				expr *l1 = expr_new(ET_VALUE, atof(buf), 0, NULL, NULL); if(!l1) return NULL;
				
				if(!next(1, p) || !isoperator(next(1, p))) {
					LOG_E("malformed expression after [%s]\n", p);
					return NULL;
				}
				
				p = next(1, p); // operator
				
				char op = *p;
				
				if(!next(1, p)) {
					LOG_E("malformed expression after [%s]\n", p);
					return NULL;
				}
				
				p = next(1, p); // right side
					
				if(isoperator1(op)) { // priority for this op
					
					expr *r1 = strtoe(p, l1); if(!r1) return NULL;
					expr *e = expr_new(ET_COMPOSED, 0, op, l1, r1); if(!e) return NULL;
					
					
				}
				else {
					
					
				}
			}
			else if() {
				
			}
			else {
				
			}
		}
	}
}




bool simplify(expr *e) {
	bool res = false;
	
	if(!e) return false;
	
	if(e->type == ET_VALUE) return false;
	
	// simplify left
	bool res_l = true;
	while(res_l && e->left->type == ET_COMPOSED) {
		res_l = simplify(e->left);
		res = res || res_l;
	}
	
	// simplify right
	bool res_r = true;
	while(res_r && e->right->type == ET_COMPOSED) {
		res_r = simplify(e->right);
		res = res || res_r;
	}
	
	// simplify current
	if(e->left->type == ET_VALUE && e->right->type == ET_VALUE) {
		e->type = ET_VALUE;
		
		if(e->op == '+') {
			e->value = e->left->value + e->right->value;
		}
		else if(e->op == '-') {
			e->value = e->left->value - e->right->value;
		}
		else if(e->op == '*') {
			e->value = e->left->value * e->right->value;
		}
		else {
			e->value = e->left->value / e->right->value;
		}
		
		free(e->left); 
		e->left = NULL;
		free(e->right);
		e->right = NULL;
		
		res = true;
	}
	
	return res;
}


expr *expr_fprintf_rec(FILE *f, expr *e) {
	if(e->type == ET_VALUE) {
		fprintf(f, "%f", e->value);
	}
	else if(e->type == ET_COMPOSED) {
		fprintf(f, "(");
		expr_fprintf_rec(f, e->left);
		fprintf(f, " %c ", e->op);
		expr_fprintf_rec(f, e->right);
		fprintf(f, ")");
	}
	else {
		LOG_E("no such expr type [%d]\n", e->type);
		return NULL;
	}
	return e;
}

void expr_fprintf(FILE *f, expr *e) {
	expr_fprintf_rec(f, e);
	fprintf(f, "\n");
}


