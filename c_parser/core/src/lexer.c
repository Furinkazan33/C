#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"



void string_write(void *s, FILE *file) {
	char *v = s;
	fprintf(file, "%s", v);
}

/* end of word */
const char *EOW = " &#{[(|`^@)]°=}+-/*$%µ!§:;,?<>\"\'\\\n\t.";
/* end of number */
const char *EON = " &#{[(|`^@)]°=}+-/*$%µ!§:;,?<>\"\'\\\n\t";


char *lex_LTT_literal(lex_token_type type) {
	switch(type) {
		case LT_NONE:
			return "NONE";
		case LT_OPERATOR:
			return "OPERATOR"; 
		case LT_TYPE:
			return "TYPE"; 
		case LT_VALUE:
			return "VALUE"; 
		case LT_KEYWORD:
			return "KEYWORD";
		case LT_COMMENT:
			return "COMMENT";
		case LT_BLANK:
			return "BLANK";
		case LT_NAME:
			return "NAME"; 
	}
	return NULL;
}

char *lex_LC_literal(lex_comment type) {
	switch(type) {
		case LT_COMMENT_NONE:
			return "COMMENT_NONE";
		case LT_COMMENT_LINE:
			return "COMMENT_LINE";
		case LT_COMMENT_BLOCK:
			return "COMMENT_BLOCK";
	}
	return NULL;
}

char *lex_LB_literal(lex_blank type) {
	switch(type) {
		case LT_BLANK_NONE:
			return "BLANK_NONE";
		case LT_BLANK_NEWLINE:
			return "BLANK_NEWLINE";
		case LT_BLANK_SPACE:
			return "BLANK_SPACE";
		case LT_BLANK_TAB:
			return "BLANK_TAB";
	}
	return NULL;
}
char *lex_LV_literal(lex_value type) {
	switch(type) {
		case LT_V_NONE:
			return "V_NONE";
		case LT_V_INT:
			return "V_INT"; 
		case LT_V_DOUBLE:
			return "V_DOUBLE"; 
		case LT_V_CHAR:
			return "V_CHAR"; 
		case LT_V_STRING:
			return "V_STRING";
	}
	return NULL;
}

char *lex_LKW_literal(lex_keyword type) {
	switch(type) {
		case LT_KW_NONE:
			return "KW_NONE";
		case LT_KW_CONST:
			return "KW_CONST";
		case LT_KW_TYPEDEF:
			return "KW_TYPEDEF";
		case LT_KW_DEFINE:
			return "KW_DEFINE";
		case LT_KW_INCLUDE:
			return "KW_INCLUDE";
		case LT_KW_RETURN:
			return "KW_RETURN";
		case LT_KW_IF:
			return "KW_IF";
		case LT_KW_ELSE:
			return "KW_ELSE";
		case LT_KW_WHILE:
			return "KW_WHILE";
		case LT_KW_SWITCH:
			return "KW_SWITCH";
		case LT_KW_CASE:
			return "KW_CASE";
		case LT_KW_BREAK:
			return "KW_BREAK";
		case LT_KW_DEFAULT:
			return "KW_DEFAULT";
		case LT_KW_IFDEF:
			return "KW_IFDEF";
		case LT_KW_ENDIF:
			return "KW_ENDIF";
	}
	return NULL;
}

char *lex_LO_literal(lex_operator type) {
	switch(type) {
		case LT_OP_NONE:
			return "OP_NONE";
		case LT_OP_DOT:
			return "OP_DOT";
		case LT_OP_EQUAL:
			return "OP_EQUAL";
		case LT_OP_PLUS:
			return "OP_PLUS";
		case LT_OP_MINUS:
			return "OP_MINUS";
		case LT_OP_DIV:
			return "OP_DIV";
		case LT_OP_STAR:
			return "OP_STAR";
		case LT_OP_PERCENT:
			return "OP_PERCENT";
		case LT_OP_AND:
			return "OP_AND";
		case LT_OP_OR:
			return "OP_OR";
		case LT_OP_NOT:
			return "OP_NOT";
		case LT_OP_LHOOK:
			return "OP_LHOOK";
		case LT_OP_RHOOK:
			return "OP_RHOOK";
		case LT_OP_LPAREN:
			return "OP_LPAREN";
		case LT_OP_RPAREN:
			return "OP_RPAREN";
		case LT_OP_LSTRIPE:
			return "OP_LSTRIPE";
		case LT_OP_RSTRIPE:
			return "OP_RSTRIPE";
		case LT_OP_LBRACKET:
			return "OP_LBRACKET";
		case LT_OP_RBRACKET:
			return "OP_RBRACKET";
		case LT_OP_COLON:
			return "OP_COLON";
		case LT_OP_SHARP:
			return "OP_SHARP";
		case LT_OP_COMMA:
			return "OP_COMMA";
		case LT_OP_SEMICOLON:
			return "OP_SEMICOLON";
		case LT_OP_QUOTES:
			return "OP_QUOTES";
		case LT_OP_QUOTE:
			return "OP_QUOTE";
	}
	return NULL;
}

char *lex_LT_literal(lex_type type) {
	switch(type) {
		case LT_T_NONE:
			return "T_NONE";
		case LT_T_VOID:
			return "T_VOID"; 
		case LT_T_INT:
			return "T_INT"; 
		case LT_T_DOUBLE:
			return "T_DOUBLE"; 
		case LT_T_CHAR:
			return "T_CHAR"; 
		case LT_T_STRUCT:
			return "T_STRUCT";
		case LT_T_ENUM:
			return "T_ENUM";
	}
	return NULL;
}

token *lex_token_new(char *string, lex_token_type type, int type2) {
	assert(string);

	token *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "lex_token_new : call to malloc token returned NULL\n");
		return NULL;
	}
	res->literal = malloc(sizeof(char) * (strlen(string) + 1));
	if(!res->literal) {
		fprintf(stderr, "lex_token_new : call to malloc string returned NULL\n");
		return NULL;
	}

	strcpy(res->literal, string);
	res->type = type;
	res->type2 = type2;

	return res;
}

void lex_string_write(void *s, void *f) {
	assert(s);
	assert(f);

	fprintf((FILE *)f, "%s", (char *)s);
}

void lex_token_write(void *tok, void *file) {
	assert(tok);
	assert(file);

	token *t = tok;
	char *type = NULL;
	char *type2 = NULL;

	type = lex_LTT_literal(t->type);

	switch(t->type) {
		case LT_NONE:
		case LT_NAME:
			break;
		case LT_OPERATOR:
			type2 = lex_LO_literal(t->type2);
			break;
		case LT_KEYWORD:
			type2 = lex_LKW_literal(t->type2);
			break;
		case LT_TYPE:
			type2 = lex_LT_literal(t->type2);
			break;
		case LT_VALUE:
			type2 = lex_LV_literal(t->type2);
			break;
		case LT_BLANK:
			type2 = lex_LB_literal(t->type2);
			break;
		case LT_COMMENT:
			type2 = lex_LC_literal(t->type2);
			break;
	}

	fprintf((FILE *)file, "[%15s][%15s][%s]\n", type, type2, t->literal);
}

void lex_token_free(void *p) {
	token *t = p;
	assert(t);
	assert(t->literal != NULL);

	free(t->literal);
	free(t);
}

/*
 *
 * Helpers
 *
 */

bool lex_is_digit(char c) {
	return c >= 48 && c <= 57;
}

bool lex_is_integer(char *s) {
	while(s && *s) {
		if(!lex_is_digit(*s)) {
			return false;
		}
		s++;
	}
	return true;
}

bool lex_is_double(char *s) {
	bool dot = false;

	while(s && *s) {
		if(lex_is_digit(*s)) {
			s++;
		}
		else if(*s == '.' && !dot) {
			dot = true;
			s++;
		}
		else {
			return false;
		}
	}

	return true;
}

bool lex_is_lit_string(char *s) {
	if(*s == '"' && *(s + strlen(s) - 1) == '"') {
		return true;
	}

	return false;
}

bool lex_is_lit_char(char *s) {
	assert(s);

	if(strlen(s) == 3 && s[0] == '\'' && s[2] == '\'') {
		return true;
	}
	if(strlen(s) == 4 && s[0] == '\'' && s[1] == '\\' && s[3] == '\'') {
		return true;
	}

	return false;
}

bool lex_EOW(char *p) {
	assert(p);

	if(*p && strchr(EOW, *p)) {
		return true;
	}
	return false;
}


/*
 *
 * Main functions
 *
 */

char *lex_read_next(char **p) {
	if(!**p) {
		return NULL;
	}

#ifdef DEBUG2
	fprintf(stdout, "lex_read_next : [%c] => ", **p);
#endif

	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	size_t len = 0;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_next : call to malloc returned NULL\n");
		return NULL;
	}

	if(lex_EOW(*p)){
		res[len++] = *(*p)++;
	}
	else {
		for( ;**p && !lex_EOW(*p); (*p)++, len++) {
			if(len >= alloc_len) {
				alloc_len *= 2;
				res = realloc(res, sizeof(char) * (alloc_len + 1));
				if(!res) {
					fprintf(stderr, "lex_read_next : call to realloc returned NULL\n");
					return NULL;
				}
			}
			res[len] = **p;
		}
	}
	res[len] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "[%s]\n", res);
#endif

	return res;
}

char *lex_read_quot(char **p, char c) {
	assert(p);
	assert(**p);

#ifdef DEBUG2
	fprintf(stdout, "lex_read_quot : [%c] => ", **p);
#endif
	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	size_t len = 0;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_quot : call to malloc returned NULL\n");
		return NULL;
	}
	res[len++] = *(*p)++;

	//TODO: multi-lines with escape char '\'
	while(**p && **p != '\n' && (**p != c || (len > 1 && *((*p)-1) == '\\'))) {
		if(len >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_quot : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[len++] = *(*p)++;
	}

	if(**p == c) {
		if(len >= alloc_len) {
			alloc_len += 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_quot : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[len++] = *(*p)++;
		res[len] = '\0';
	}
	else {
		fprintf(stderr, "lex_read_quot : string literal format error\n");
		return NULL;
	}

#ifdef DEBUG2
	fprintf(stdout, "[%s]\n", res);
#endif

	return res;
}

bool lex_EON(char *p) {
	assert(p);

	if(*p && strchr(EON, *p)) {
		return true;
	}
	return false;
}

char *lex_read_numb(char **p) {
	assert(p);
	assert(**p);

#ifdef DEBUG2
	fprintf(stdout, "lex_read_numb : [%c] => ", **p);
#endif

	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	size_t len = 0;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_numb : call to malloc returned NULL\n");
		return NULL;
	}

	while(**p && !lex_EON(*p)) {
		if(len >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_numb : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[len++] = *(*p)++;
	}
	if(lex_EON(*p)) {
		res[len] = '\0';
	}
	else {
		fprintf(stderr, "lex_read_numb : format error\n");
		return NULL;
	}

#ifdef DEBUG2
	fprintf(stdout, "[%s]\n", res);
#endif

	return res;
}

char *lex_read_newl(char **p) {
	assert(p);
	assert(**p);
	assert(**p == '\n');

	char *res = malloc(sizeof(char) * 2);
	if(!res) {
		fprintf(stderr, "lex_read_newl : call to malloc returned NULL\n");
		return NULL;
	}

	(*p)++;
	res[0] = '\n';
	res[1] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "lex_read_newl : [\\NEWLINE] => [\\NEWLINE]\n");
#endif

	return res;
}

char *lex_read_space(char **p) {
	assert(p);
	assert(**p);
	assert(**p == ' ');

	char *res = malloc(sizeof(char) * 2);
	if(!res) {
		fprintf(stderr, "lex_read_space : call to malloc returned NULL\n");
		return NULL;
	}

	(*p)++;
	res[0] = ' ';
	res[1] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "lex_read_space : [\\SPACE] => [\\SPACE]\n");
#endif

	return res;
}

char *lex_read_comment(char **p) {
	assert(p);
	assert(**p);
	assert(**p == '/' && *((*p)+1) == '/');

	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_comment : call to malloc returned NULL\n");
		return NULL;
	}

	size_t i;
	for(i = 0; **p && **p != '\n'; (*p)++, i++) {
		if(i >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_comment : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[i] = **p;
	}
	res[i] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "lex_read_comment : [//] => [%s]\n", res);
#endif

	return res;
}

char *lex_read_comment_block(char **p) {
	assert(p);
	assert(**p);
	assert(**p == '/' && *((*p)+1) == '*');

	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_comment_block : call to malloc returned NULL\n");
		return NULL;
	}

	size_t i;
	for(i = 0; **p && *((*p)+1) && (**p != '*' || *((*p)+1) != '/'); (*p)++, i++) {
		if(i >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_comment_block : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[i] = **p;
	}

	if(**p != '*' || *((*p)+1) != '/') {
		fprintf(stderr, "lex_read_comment_block : comment block format error\n");
		return NULL;
	}

	if(i + 2 >= alloc_len) {
		alloc_len += 2;
		res = realloc(res, sizeof(char) * (alloc_len + 1));
		if(!res) {
			fprintf(stderr, "lex_read_comment_block : call to realloc returned NULL\n");
			return NULL;
		}
	}
	res[i] = *(*p)++;
	res[i+1] = *(*p)++;
	res[i+2] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "lex_read_comment_block : [/*] => [%s]\n", res);
#endif

	return res;
}

char *lex_read_tab(char **p) {
	assert(p);
	assert(**p);
	assert(**p == '\t');

	char *res = malloc(sizeof(char) * 2);
	if(!res) {
		fprintf(stderr, "lex_read_tab : call to malloc returned NULL\n");
		return NULL;
	}

	(*p)++;
	res[0] = '\t';
	res[1] = '\0';

#ifdef DEBUG2
	fprintf(stdout, "lex_read_tab : [\\TAB] => [\\TAB]\n");
#endif

	return res;
}

list *lex_code_to_strings(char *code, int *n) {
	assert(code);

	list *res = NULL;
	list *p_res = NULL;
	char *string = NULL;
	char *p = code;
	char *pos = NULL;
	*n = 0;

	while(*p) {
		pos = p;

		if(*p == ' ') {
			string = lex_read_space(&p);
		}
		else if(*p == '/' && *(p+1) == '/') {
			string = lex_read_comment(&p);
		}
		else if(*p == '/' && *(p+1) == '*') {
			string = lex_read_comment_block(&p);
		}
		else if(*p == '\t') {
			string = lex_read_tab(&p);
		}
		else if(*p == '\n') {
			string = lex_read_newl(&p);
		}
		else if(*p == '"') {
			string = lex_read_quot(&p, '"');
		}
		else if(*p == '\'') {
			string = lex_read_quot(&p, '\'');
		}
		else if(lex_is_digit(*p)){
			string = lex_read_numb(&p);
		}
		else {
			string = lex_read_next(&p);
		}
		if(!*p) {
			break;
		}
		if(!string) {
			fprintf(stderr, "lex_code_to_strings : error near %s\n", pos);
			return NULL;
		}
		if(!res) {
			res = list_new(string);
			if(!res) {
				fprintf(stderr, "lex_code_to_strings : call to list_new returned NULL\n");
				return NULL;
			}
			p_res = res;
		}
		else {
			p_res = list_insert_after(p_res, string);
			if(!p_res) {
				fprintf(stderr, "lex_code_to_strings : call to list_append returned NULL\n");
				return NULL;
			}
		}
		(*n)++;
	}

	return res;
}



/* new token from string */
void *lex_string_to_token(void *string) {
	assert(string);

	char *s = string;

#ifdef DEBUG2
	fprintf(stdout, "lex_string_to_token : [%s][%ld]\n", s, strlen(s));
#endif
	token *res = lex_token_new(s, LT_NONE, LT_NONE);
	if(!res) {
		fprintf(stderr, "lex_string_to_token : call to lex_token_new returned NULL\n");
		return NULL;
	}
	if(!lex_token_set_types(res)) {
		fprintf(stderr, "lex_string_to_token : call to lex_token_set_types returned NULL\n");
		return NULL;
	}
#ifdef DEBUG2
	fprintf(stdout, "lex_string_to_token : ");
	lex_token_write(res, stdout);
	fflush(stdout);
#endif

	return res;
}

token *lex_token_set_types(token *t) {
	assert(t);
	assert(t->literal);

	if(lex_is_lit_string(t->literal)) {
		t->type = LT_VALUE;
		t->type2 = LT_V_STRING;
		return t;
	}
	if(lex_is_lit_char(t->literal)) {
		t->type = LT_VALUE;
		t->type2 = LT_V_CHAR;
		return t;
	}


	if(t->literal[0] == '/' && t->literal[1] == '/') {
		t->type = LT_COMMENT;
		t->type2 = LT_COMMENT_LINE;
		return t;
	}
	if(t->literal[0] == '/' && t->literal[1] == '*') {
		t->type = LT_COMMENT;
		t->type2 = LT_COMMENT_BLOCK;
		return t;
	}
	if(t->literal[0] == '\n') {
		t->literal[0] = ' '; // for displaying purpose
		t->type = LT_BLANK;
		t->type2 = LT_BLANK_NEWLINE;
		return t;
	}
	if(t->literal[0] == ' ') {
		t->type = LT_BLANK;
		t->type2 = LT_BLANK_SPACE;
		return t;
	}
	if(t->literal[0] == '\t') {
		t->type = LT_BLANK;
		t->type2 = LT_BLANK_TAB;
		return t;
	}
	if(t->literal[0] == '*') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_STAR;
		return t;
	}
	if(t->literal[0] == '&') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_AND;
		return t;
	}
	if(t->literal[0] == '|') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_OR;
		return t;
	}
	if(t->literal[0] == '!') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_NOT;
		return t;
	}
	if(t->literal[0] == '#') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_SHARP;
		return t;
	}
	if(t->literal[0] == '"') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_QUOTES;
		return t;
	}
	if(t->literal[0] == '\'') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_QUOTE;
		return t;
	}
	if(t->literal[0] == '.') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_DOT;
		return t;
	}
	if(t->literal[0] == ',') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_COMMA;
		return t;
	}
	if(t->literal[0] == ';') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_SEMICOLON;
		return t;
	}
	if(t->literal[0] == ':') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_COLON;
		return t;
	}
	if(t->literal[0] == '=') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_EQUAL;
		return t;
	}
	if(t->literal[0] == '+') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_PLUS;
		return t;
	}
	if(t->literal[0] == '-') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_MINUS;
		return t;
	}
	if(t->literal[0] == '[') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_LHOOK;
		return t;
	}
	if(t->literal[0] == ']') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_RHOOK;
		return t;
	}
	if(t->literal[0] == '(') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_LPAREN;
		return t;
	}
	if(t->literal[0] == ')') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_RPAREN;
		return t;
	}
	if(t->literal[0] == '<') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_LSTRIPE;
		return t;
	}
	if(t->literal[0] == '>') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_RSTRIPE;
		return t;
	}
	if(t->literal[0] == '{') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_LBRACKET;
		return t;
	}
	if(t->literal[0] == '}') {
		t->type = LT_OPERATOR;
		t->type2 = LT_OP_RBRACKET;
		return t;
	}



	if(lex_is_integer(t->literal)) {
		t->type = LT_VALUE;
		t->type2 = LT_V_INT;
		return t;
	}
	if(lex_is_double(t->literal)) {
		t->type = LT_VALUE;
		t->type2 = LT_V_DOUBLE;
		return t;
	}


	if(!strcmp(t->literal, "int")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_INT;
		return t;
	}
	if(!strcmp(t->literal, "double")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_DOUBLE;
		return t;
	}
	if(!strcmp(t->literal, "char")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_CHAR;
		return t;
	}
	if(!strcmp(t->literal, "void")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_VOID;
		return t;
	}
	if(!strcmp(t->literal, "struct")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_STRUCT;
		return t;
	}
	if(!strcmp(t->literal, "enum")) {
		t->type = LT_TYPE;
		t->type2 = LT_T_ENUM;
		return t;
	}
	if(!strcmp(t->literal, "const")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_CONST;
		return t;
	}
	if(!strcmp(t->literal, "typedef")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_TYPEDEF;
		return t;
	}
	if(!strcmp(t->literal, "define")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_DEFINE;
		return t;
	}
	if(!strcmp(t->literal, "include")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_INCLUDE;
		return t;
	}
	if(!strcmp(t->literal, "return")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_RETURN;
		return t;
	}
	if(!strcmp(t->literal, "if")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_IF;
		return t;
	}
	if(!strcmp(t->literal, "else")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_ELSE;	
		return t;
	}
	if(!strcmp(t->literal, "case")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_CASE;
		return t;
	}
	if(!strcmp(t->literal, "break")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_BREAK;
		return t;
	}
	if(!strcmp(t->literal, "default")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_DEFAULT;
		return t;
	}
	if(!strcmp(t->literal, "switch")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_SWITCH;
		return t;
	}
	if(!strcmp(t->literal, "ifdef")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_IFDEF;
		return t;
	}
	if(!strcmp(t->literal, "endif")) {
		t->type = LT_KEYWORD;
		t->type2 = LT_KW_ENDIF;
		return t;
	}
	/* default */
	if(t->type == LT_NONE) {
		t->type = LT_NAME;
		t->type2 = LT_NAME;
		return t;
	}

	return NULL;
}

list *lex_strings_to_tokens(list *strings) {
	assert(strings);

	/* transform every string to token */
	list *tokens = list_copy(strings, lex_string_to_token);
	if(!tokens) {
		fprintf(stdout, "lex_strings_to_tokens : call to list_copy returned NULL\n");
		return NULL;
	}

	return tokens;
}



