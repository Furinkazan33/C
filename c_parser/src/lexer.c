#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

typedef struct tok_single {
	LEX_type type;
	char c;
} tok_single;

static const tok_single tok_singles[] = { 
	{ L_EQUAL, '=' }, 
	{ L_LPAREN, '(' }, 
	{ L_RPAREN, ')' }, 
	{ L_LHOOK, '[' },
	{ L_RHOOK, ']' },
	{ L_LBRACKET, '{' },
	{ L_RBRACKET, '}' },
	{ L_COMMA, ',' }, 
	{ L_SEMI, ';' }, 
	// TODO
	{ 0, 0 } };

static const char *tok_types[] = { "int", "char", "long", "double", "const", "enum", "float", "static", 
	"enum", "struct", "union", "void", "short", "signed",
	0 };

static const char *tok_reserved[] = {
	"||", 
	"alignas", "alignof", "break", "case", "continue", "default", "do", "else", "false", "for", "goto", "if", 
	"inline", "register", "restrict", "return", 
	"sizeof", "switch", "thread_local", "true", "typedef", "typeof", "unsigned", "while", 
	0 };


char *ltok_type_to_s(LEX_type type) {
	switch(type) {
		case L_NONE: 
			return "L_NONE";
		case L_TYPE:
			return "L_TYPE";
		case L_DIRECTIVE:
			return "L_DIRECTIVE";
		case L_RESERVED:
			return "L_RESERVED";
		case L_COMMENT:
			return "L_COMMENT";
		case L_COMMENTS:
			return "L_COMMENTS";
		case L_VARNAME:
			return "L_VARNAME";
		case L_ARROW:
			return "L_ARROW";
		case L_BLANK:
			return "L_BLANK";
		case L_LPAREN:
			return "L_LPAREN";
		case L_RPAREN:
			return "L_RPAREN";
		case L_LBRACKET:
			return "L_LBRACKET";
		case L_RBRACKET:
			return "L_RBRACKET";
		case L_LHOOK:
			return "L_LHOOK";
		case L_RHOOK:
			return "L_RHOOK";
		case L_QUOTE:
			return "L_QUOTE";
		case L_DQUOTE:
			return "L_DQUOTE";
		case L_EQUAL:
			return "L_EQUAL";
		case L_COMMA:
			return "L_COMMA";
		case L_COLON:
			return "L_COLON";
		case L_SEMI:
			return "L_SEMI";
		case L_DOT:
			return "L_DOT";
		case L_SPACE:
			return "L_SPACE";
		case L_BSn:
			return "L_BSn";
		case L_BSt: 
			return "L_BSt";
		case L_LIT_INT:
			return "L_LIT_INT";
	}
	return NULL;
}

LEX_token *ltok_new(LEX_type t, char *s, char *e) {
	LEX_token *res = malloc(sizeof(*res));
	if(!res) {}
	res->type = t;
	res->start = s;
	res->end = e;
	res->len = e - s + 1;

	return res;
}

void ltok_print(LEX_token *t) {
	char *tmp = malloc(sizeof(char) * (t->len + 2));
	strncpy(tmp, t->start, t->len);
	tmp[t->len] = '\0';
	printf("%s\n", tmp);
	free(tmp);
}

int cis_digit(char c) {
	return c >= '0' && c <= '9';
}

int cis_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int cis_alnum(char c) {
	return cis_digit(c) || cis_alpha(c);
}

int cis_varname(char c) {
	return c == '_' || cis_alnum(c);
}

int cis_blank(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == 'b';
}


LEX_token *read_blanks(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	while(cis_blank(*end)) {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_BLANK, *start, end - 1);
		*start = end; // next token
	}

	return res;
}

LEX_token *read_single(char **start, LEX_type type, char c) {
	LEX_token *res = NULL;
	char *end = *start;

	if(*end == c) {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(type, *start, end - 1);
		*start = end; // next token
	}

	return res;
}


LEX_token *read_semi(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	if(*end == ';') {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_SEMI, *start, end - 1);
		*start = end; // next token
	}

	return res;
}


LEX_token *read_equal(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	if(*end == '=') {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_EQUAL, *start, end - 1);
		*start = end; // next token
	}

	return res;
}

LEX_token *read_comment(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	if(*end != '/' || *(end + 1) != '/') {
		return NULL;
	}

	while(*end && *end != '\n') {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_COMMENT, *start, end - 1);
		*start = end; // next token
	}

	return res;
}

LEX_token *read_comments(char **start) {
	LEX_token *res = NULL;
	char *end;

	if(**start != '/' || *((*start) + 1) != '*') {
		return NULL;
	}

	for(end = (*start) + 2; *end && !(*end == '*' && *(end + 1) == '/'); end++) {
		;
	}

	if(!*end) {
		LOGE("misformed multi-lines comment");
		return NULL;
	}
	end += 2;

	res	= ltok_new(L_COMMENTS, *start, end - 1);
	*start = end; // next token

	return res;
}


LEX_token *read_directive(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	if(*end != '#') {
		return NULL;
	}
	end++;

	while(*end && *end != '\n') {
		end++;
	}

	res	= ltok_new(L_DIRECTIVE, *start, end - 1);
	*start = end; // next token

	return res;
}

// read types and advance to next word if ok
LEX_token *read_type(char **start, const char *word) {
	if(!strncmp(*start, word, strlen(word))) {
		LEX_token *res = ltok_new(L_TYPE, *start, (*start) + strlen(word) - 1);
		(*start) += strlen(word);
		return res;
	}
	return NULL;
}


// read reserved word and advance to next word if ok
LEX_token *read_reserved(char **start, const char *word) {
	if(!strncmp(*start, word, strlen(word))) {
		LEX_token *res = ltok_new(L_RESERVED, *start, (*start) + strlen(word) - 1);
		(*start) += strlen(word);
		return res;
	}
	return NULL;
}

LEX_token *read_varname(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	while(cis_varname(*end)) {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_VARNAME, *start, end - 1);
		*start = end; // next token
	}

	return res;
}

LEX_token *read_value_lit_int(char **start) {
	LEX_token *res = NULL;
	char *end = *start;

	while(cis_digit(*end)) {
		end++;
	}

	if(end != *start) {
		res	= ltok_new(L_LIT_INT, *start, end - 1);
		*start = end; // next token
	}

	return res;
}



arrayptr *lexer(char *text) {
	arrayptr *res = arrayptr_new(50);
	LEX_token *tok;

	while(text && *text) {
		tok = NULL;
		if(!tok) { tok = read_blanks(&text); }
		if(!tok) { tok = read_comment(&text); }
		if(!tok) { tok = read_comments(&text); }
		if(!tok) { tok = read_directive(&text); }

		for(size_t i = 0; tok_types[i]; i++) {
			if(!tok) { tok = read_type(&text, tok_types[i]); }
		}

		for(size_t i = 0; tok_reserved[i]; i++) {
			if(!tok) { tok = read_reserved(&text, tok_reserved[i]); }
		}

		for(size_t i = 0; tok_singles[i].type; i++) {
			if(!tok) { tok = read_single(&text, tok_singles[i].type, tok_singles[i].c); }
		}

		if(!tok) { tok = read_value_lit_int(&text); }
		//if(!tok) { tok = read_value_lit_float(&text); }
		//if(!tok) { tok = read_value_lit_char(&text); }
		//if(!tok) { tok = read_value_lit_string(&text); }

		if(!tok) { tok = read_varname(&text); }

		if(!tok) {
			LOGE("error near :");
			fprintf(stderr, "%s\n", text);

			ARRAYPTR_FREE_F(res, free);

			return NULL;
		}

		arrayptr_append(res, tok);


#ifdef DEBUG
		char tmp[128] = {0};
		strncpy(tmp, tok->start, tok->len);
		printf("READ:[%2d][%10s][%s]\n", tok->type, ltok_type_to_s(tok->type), tmp);
#endif
	}

	return res;
}



