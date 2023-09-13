#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"



void string_write(void *s, FILE *file) {
	char *v = s;
	fprintf(file, "%s", v);
}

const char *EOW = " &#{[(|`^@)]°=}+-/*$%µ!§:;.,?<>\"\'\\\n\t";

char *lex_type_literal[] = {
	[LT_NONE] = "NONE",
	[LT_OPERATOR] = "OPERATOR", 
		[LT_OP_SHARP] = "OP_SHARP", 
		[LT_OP_DOT] = "OP_DOT", 
		[LT_OP_EQUAL] = "OP_EQUAL",
		[LT_OP_PLUS] = "OP_PLUS",
		[LT_OP_MINUS] = "OP_MINUS",
		[LT_OP_DIV] = "OP_DIV",
		[LT_OP_STAR] = "OP_STAR",
		[LT_OP_DIVE] = "OP_DIVE",
		[LT_OP_PERCENT] = "OP_PERCENT",
		[LT_OP_AND] = "OP_AND",
		[LT_OP_OR] = "OP_OR",
		[LT_OP_NOT] = "OP_NOT",
		[LT_OP_LHOOK] = "OP_LHOOK",
		[LT_OP_RHOOK] = "OP_RHOOK",
		[LT_OP_LPAREN] = "OP_LPAREN",
		[LT_OP_RPAREN] = "OP_RPAREN",
		[LT_OP_LSTRIPE] = "OP_LSTRIPE",
		[LT_OP_RSTRIPE] = "OP_RSTRIPE",
		[LT_OP_LBRACKET] = "OP_LBRACKET",
		[LT_OP_RBRACKET] = "OP_RBRACKET",
		[LT_OP_COLON] = "OP_COLON", 
		[LT_OP_COMMA] = "OP_COMMA",
		[LT_OP_SEMICOLON] = "OP_SEMICOLON",
	[LT_TYPE] = "TYPE", 
		[LT_T_VOID] = "T_VOID", 
		[LT_T_INT] = "T_INT", 
		[LT_T_DOUBLE] = "T_DOUBLE", 
		[LT_T_CHAR] = "T_CHAR", 
		[LT_T_STRUCT] = "T_STRUCT",
		[LT_T_ENUM] = "T_ENUM",
	[LT_VALUE] = "VALUE", 
		[LT_V_INT] = "V_INT", 
		[LT_V_DOUBLE] = "V_DOUBLE", 
		[LT_V_CHAR] = "V_CHAR", 
		[LT_V_STRING] = "V_STRING",
	[LT_KEYWORD] = "KEYWORD", 
		[LT_KW_CONST] = "KW_CONST", 
		[LT_KW_TYPEDEF] = "KW_TYPEDEF", 
		[LT_KW_INCLUDE] = "KW_INCLUDE", 
		[LT_KW_DEFINE] = "KW_DEFINE", 
		[LT_KW_SWITCH] = "KW_SWITCH", 
		[LT_KW_CASE] = "KW_CASE", 
		[LT_KW_BREAK] = "KW_BREAK", 
		[LT_KW_DEFAULT] = "KW_DEFAULT", 
		[LT_KW_IF] = "KW_IF", 
		[LT_KW_IFDEF] = "KW_IFDEF", 
		[LT_KW_ELSE] = "KW_ELSE", 
		[LT_KW_ENDIF] = "KW_ENDIF", 
		[LT_KW_WHILE] = "KW_WHILE",
		[LT_KW_RETURN] = "KW_RETURN",
	[LT_NAME] = "NAME", 
	[LT_COMMENT] = "COMMENT",
		[LT_COMMENT_LINE] = "LINE",
		[LT_COMMENT_BLOCK] = "BLOCK",
	[LT_BLANK] = "BLANK",
		[LT_BLANK_NEWLINE] = "NEWLINE",
		[LT_BLANK_SPACE] = "SPACE",
		[LT_BLANK_TAB] = "TAB",
};

char *lex_type_lit(lex_type t) {
	return lex_type_literal[t];
}


token *lex_token_new(char *string, lex_type type) {
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

	return res;
}

void lex_string_write(void *s, void *f) {
	fprintf((FILE *)f, "%s", (char *)s);
}

void lex_token_write(void *tok, void *file) {
	token *t = tok;
	fprintf((FILE *)file, "[%15s][%15s][%s]\n", lex_type_lit(t->type), lex_type_lit(t->type2), t->literal);
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


void lex_ignore_blanks(char **code) {
	while(**code == ' ' || **code == '\t') {
		(*code)++;
	}
}

bool lex_EOW(char *p) {
	assert(p);
	if(!*p) {
		return true;
	}
	if(strchr(EOW, *p)) {
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

	while(**p && (**p != c || (len > 1 && *((*p)-1) == '\\'))) {
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

char *lex_read_sharp(char **p) {
	assert(p);
	assert(**p);

#ifdef DEBUG2
	fprintf(stdout, "lex_read_sharp : [%c] => ", **p);
#endif

	size_t alloc_len = LEX_TOK_INIT_LEN_ALLOC;
	size_t len = 0;
	char *res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		fprintf(stderr, "lex_read_sharp : call to malloc returned NULL\n");
		return NULL;
	}

	while(**p && **p != '\n') {
		if(len >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stderr, "lex_read_sharp : call to realloc returned NULL\n");
				return NULL;
			}
		}
		res[len++] = *(*p)++;
	}
	if(**p == '\n') {
		res[len] = '\0';
	}
	else {
		fprintf(stderr, "lex_read_sharp : preproc statement format error\n");
		return NULL;
	}

#ifdef DEBUG2
	fprintf(stdout, "[%s]\n", res);
#endif

	return res;
}

const char *EON = " &#{[(|`^@)]°=}+-/*$%µ!§:;,?<>\"\'\\\n\t";

bool lex_EON(char *p) {
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
		fprintf(stderr, "lex_read_numb : preproc statement format error\n");
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
	for(i = 0; **p && (**p != '*' || *((*p)+1) != '/'); (*p)++, i++) {
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
	*n = 0;

	while(*p) {
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
			fprintf(stderr, "lex_code_to_strings : reading returned NULL\n");
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
	token *res = lex_token_new(s, LT_NONE);
	if(!res) {
		fprintf(stderr, "lex_string_to_token : call to lex_token_new returned NULL\n");
		return NULL;
	}
	if(!lex_token_set_types(res)) {
		fprintf(stderr, "lex_string_to_token : call to lex_token_set_types returned NULL\n");
		return NULL;
	}
#ifdef DEBUG2
	fprintf(stdout, "lex_string_to_token : [%s][%s][%s]\n", lex_type_lit(res->type), lex_type_lit(res->type2), res->literal);
	fflush(stdout);
#endif

	return res;
}

token *lex_token_set_types(token *t) {
	assert(t);
	assert(t->literal);

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
	/* default */
	if(t->type == LT_NONE) {
		t->type = LT_NAME;
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



