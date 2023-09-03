#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lexer.h"
#include "../include/array.h"


// TODO: get_r_value after reading '=' operator


const char token_type_literal[][LEX_MAX_TOKEN_LEN] = {
    [T_TYPE_INT]        =    "int",
    [T_TYPE_CHAR]       =    "char",
    [T_TYPE_VOID]       =    "void",
	[T_TYPE_INT_S]       =    "int *",
	[T_TYPE_CHAR_S]       =    "char *",
	[T_TYPE_VOID_S]       =    "void *",
	[T_AND]			=	"&&",
	[T_OR]			=	"||",
    [T_L_STRIPE]   	=    "<",
    [T_R_STRIPE]   	=    ">",
    [T_COMMA]      	=    ",",
    [T_SEMICOLON]  	=    ";",
    [T_EQUAL]      	=    "=",
    [T_SPACE]      	=    " ",
    [T_QUOTES]    	=    "\"",
    [T_L_BRACKET]  	=    "{",
    [T_R_BRACKET]  	=    "}",
    [T_DOT]        	=    ".",
    [T_NEWLINE]        	=    "\n",
    [T_EOS]        	=    "\0",
    [T_TAB]        	=    "\t",
    [T_HYPHEN]  	=    "-",
    [T_QUOTE]      	=    "'",
	[T_L_PAREN]		=	"(",
	[T_R_PAREN]		=	")",
	[T_SHARP]		=	"#",
	[T_PLUS]		=	"+",
	[T_MINUS]		=	"-",
	[T_STAR]		=	"*",
	[T_SLASH]		=	"/",
	[T_AMPER]		=	"&",
    [T_STRUCT]          =    "struct",
    [T_TYPEDEF]         =    "typedef",
    [T_ENUM]            =    "enum",
    [T_INCLUDE]         =    "include",
    [T_DEFINE]          =    "define",
	[T_RETURN]			=	 "return",
	[T_EXCL]			=	"!",
	[T_IF]				=	"if",
	[T_ELSE]			=	"else",
    [T_ESCAPE]    	=    "\\",

	/* End of tab */
	[T_EOT] = "__RESERVED__EOT",
};

/*
    Token types literals for debug printings
*/
const char token_type_debug[][LEX_MAX_TOKEN_LEN] = {
    [T_TYPE_INT]        =    "T_TYPE_INT",
    [T_TYPE_CHAR]       =    "T_TYPE_CHAR",
    [T_TYPE_VOID]       =    "T_TYPE_VOID",
	[T_TYPE_INT_S]       =    "T_TYPE_INT_S",
	[T_TYPE_CHAR_S]       =    "T_TYPE_CHAR_S",
	[T_TYPE_VOID_S]       =    "T_TYPE_VOID_S",
    [T_L_STRIPE]   =    "T_L_STRIPE",
    [T_R_STRIPE]   =    "T_R_STRIPE",
    [T_COMMA]      =    "T_COMMA",
    [T_SEMICOLON]  =    "T_SEMICOLON",
    [T_EQUAL]      =    "T_EQUAL",
    [T_SPACE]      =    "T_SPACE",
    [T_ESCAPE]    =    "T_ESCAPE",
    [T_QUOTES]    =    "T_QUOTES",
    [T_L_BRACKET]  =    "T_L_BRACKET",
    [T_R_BRACKET]  =    "T_R_BRACKET",
    [T_DOT]        =    "T_DOT",
    [T_NEWLINE]        =    "T_NEWLINE",
    [T_EOS]        =    "T_EOS",
    [T_TAB]        =    "T_TAB",
    [T_HYPHEN]     =    "T_HYPHEN",
    [T_QUOTE]      =    "T_QUOTE",
	[T_L_PAREN]		=	"T_L_PAREN",
	[T_R_PAREN]		=	"T_R_PAREN",
	[T_SHARP]		=	"T_SHARP",
	[T_PLUS]		=	"T_PLUS",
	[T_MINUS]		=	"T_MINUS",
	[T_STAR]		=	"T_STAR",
	[T_SLASH]		=	"T_SLASH",
	[T_AMPER]		=	"T_AMPER",
    [T_STRUCT]          =    "T_STRUCT",
    [T_TYPEDEF]         =    "T_TYPEDEF",
    [T_ENUM]            =    "T_ENUM",
    [T_INCLUDE]         =    "T_INCLUDE",
    [T_DEFINE]          =    "T_DEFINE",
	[T_RETURN]			=	 "T_RETURN",
	[T_AND]			=	"T_AND",
	[T_OR]			=	"T_OR",
	[T_EXCL]				=	"T_EXCL",
	[T_IF]				=	"T_IF",
	[T_ELSE]			=	"T_ELSE",
    [T_LIT_INT] 	=    "T_LIT_INT",
    [T_LIT_DOUBLE] 	=    "T_LIT_DOUBLE",
    [T_LIT_STRING] 	=    "T_LIT_STRING",
    [T_VARNAME]        =    "T_VARNAME",

	/* End of tab */
	[T_EOT] = "__RESERVED__EOT",
};

const char *end_word[] = {
	/* 2 chars tokens  */
	token_type_literal[T_AND],
	token_type_literal[T_OR],

	/* 1 char tokens  */
	token_type_literal[T_EXCL],
    token_type_literal[T_L_STRIPE],
    token_type_literal[T_R_STRIPE],
    token_type_literal[T_COMMA],
    token_type_literal[T_SEMICOLON],
    token_type_literal[T_EQUAL],
    token_type_literal[T_SPACE],
    token_type_literal[T_QUOTES],
    token_type_literal[T_L_BRACKET],
    token_type_literal[T_R_BRACKET],
    token_type_literal[T_DOT],
    token_type_literal[T_NEWLINE],
    token_type_literal[T_EOS],
    token_type_literal[T_TAB],
    token_type_literal[T_HYPHEN],
    token_type_literal[T_QUOTE],
	token_type_literal[T_L_PAREN],
	token_type_literal[T_R_PAREN],
	token_type_literal[T_SHARP],
	token_type_literal[T_PLUS],
	token_type_literal[T_MINUS],
	token_type_literal[T_STAR],
	token_type_literal[T_SLASH],
	token_type_literal[T_AMPER],
    
	//token_type_literal[T_ESCAPE],

	/* End of tab */
	token_type_literal[T_EOT]
};


const char *ignore[] = {
	token_type_literal[T_SPACE],
	token_type_literal[T_NEWLINE],
	token_type_literal[T_TAB],

	/* End of tab */
	token_type_literal[T_EOT]
};

/* Char is a digit */
bool char_is_int(char c) {
	return c >= 48 && c <= 57;
}

/* Every char is a digit */
bool string_is_int(char *s) {
	while(s && *s) {
		if(!char_is_int(*s)) {
			return false;
		}
		s++;
	}
	return true;
}

bool string_is_double(char *s) {
	bool dot = false;
	bool end = false;
	int n_dot = 0;

	while(s && *s) {
		if(char_is_int(*s)) {
			if(dot) {
				if(end) {
					return false;
				}
				else {
					end = true;
				}
			}
			s++;
		}
		else if(*s == '.' && !end) {
			dot = true;
			n_dot++;
			s++;
		}
		else {
			return false;
		}
	}

	return n_dot <= 1;
}

bool string_is_string(char *s) {
	if(strlen(s) < 2) {
		return false;
	}
	if(*s != '"' || *(s + strlen(s) - 1) != '"') {
		return false;
	}
	return true;
}

/* Index is not the last */
bool not_EOT_literal(int i) {
	return strcmp(token_type_literal[i], token_type_literal[T_EOT]) != 0;
}

/* Index is not the last */
bool not_EOT_debug(int i) {
	return strcmp(token_type_debug[i], token_type_debug[T_EOT]) != 0;
}

/* Index is not the last */
bool not_EOT_EOW(int i) {
	return strcmp(end_word[i], token_type_literal[T_EOT]) != 0;
}

/* Index is not the last */
bool not_EOT_ignore(int i) {
	return strcmp(ignore[i], token_type_literal[T_EOT]) != 0;
}

/* Defines what an end of word char is */
bool char_is_EOW(char c) {
	int i;

	for(i = 0; not_EOT_EOW(i); i++) {
		if(c == end_word[i][0]) {
			return true;
		}
	}
	return false;
}

/* Defines what an empty char is  */
bool char_is_ignore(char c) {
	int i;

	for(i = 0; not_EOT_ignore(i); i++) {
		if(c == ignore[i][0]) {
			return true;
		}
	}
	return false;
}

/* Defines what an end of string char is  */
bool char_is_EOS(char c) {
	return c == token_type_literal[T_EOS][0];
}

/* Defines what a valid char is  */
bool char_is_valid(char c) {
	return !char_is_EOS(c) && !char_is_EOW(c) && !char_is_ignore(c);
}

/* Get type from string 
 * TODO: improve by : 
 *  - sorting token_type_literal
 *  - fuse string_is_int and string_is_double
 */
lex_type lex_strtot(char *expression) {
	int i;

	if(strcmp(expression, ".") == 0) {
		return T_DOT;
	}
	if(string_is_string(expression)) {
		return T_LIT_STRING;
	}
	if(strcmp(expression, "\"") == 0) {
		return T_QUOTES;
	}
	if(string_is_int(expression)) {
		return T_LIT_INT;
	}
	if(string_is_double(expression)) {
		return T_LIT_DOUBLE;
	}
	for(i = 0; not_EOT_literal(i); i++) {
		if(strcmp(expression, token_type_literal[i]) == 0) {
			return (lex_type) i;
		}
	}

	return T_VARNAME; /* If no token recognize, then it's a varname */
}

/* Get literal from type */
const char *lex_ttostr(lex_type type) {
	return token_type_debug[type];
}

/* Pointer to next non-valid char */
void pass_valid(char **p) {
	while(char_is_valid(**p)) {
		(*p)++;
	}
}

/* Pointer to next non-empty char */
void pass_ignore(char **p) {
	while(char_is_ignore(**p)) {
		(*p)++;
	}
}

/* Advancing pointer till new type of word (valid, ignore, EOW)  */
void next_word(char **p) {
	if(char_is_valid(**p)) {
		pass_valid(p);
		return;
	}

	if(char_is_ignore(**p)) {
		pass_ignore(p);
		return;
	}

	if(char_is_EOW(**p)) {
		(*p)++;
		return;
	}
}

token *lex_tok_new(char *string, lex_type type) {
	token *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "lex_tok_new : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(res->literal, string);
	res->type = type;

	return res;
}

void token_write(void *t, FILE *file) {
	const char *lit, *debug;
	lit = ((token *)t)->literal;
	debug = lex_ttostr(((token *)t)->type);
	fprintf(file, "%d:%s:%s\n", ((token *)t)->type, debug, lit);
}

//TODO: implement
//list context:
// - inside string => read until end
// - end of word read
// - escape char read
// - varname
bool lex_add_next(char *token_as_string, size_t len, char *p) {
	//if token_as_string not recognize as token, return true

	return true;
}


/* Read code and return an array of tokens.
 * Chars are added to new token 1 by 1
 */
array *lex_strtok2(char *code) {
	token *token;			// token to add to array
	lex_type type;			// type of token
	array *tokens;			// result

	/* tokens is an array of char * */
	tokens = array_new(128);
	if(!tokens) {
		fprintf(stderr, "lex_strtok2 : call to array_new returned NULL\n");
		return NULL;
	}
	array_set_free(tokens, (void (*)(void *))free);
	array_set_write(tokens, token_write);

	/* alloc first string token */
	size_t alloc_len = 8;
	size_t len = 0;
	char *token_as_string = malloc(sizeof(char) * (alloc_len + 1));
	if(!token_as_string) {
		fprintf(stderr, "lex_strtok2 : call to first malloc returned NULL\n");
		return NULL;
	}

	/* read code until end */
	char *p = code;
	while(p && *p) {
		// add next char ?
		if(lex_add_next(token_as_string, len, p)) {
			token_as_string[len++] = *p++;
			token_as_string[len] = '\0';

			/* realloc if needed */
			if(len >= alloc_len) {
				alloc_len *= 2;
				token_as_string = realloc(token_as_string, alloc_len);
				if(!token_as_string) {
					fprintf(stderr, "lex_strtok2 : call to realloc returned NULL\n");
					return NULL;
				}
			}
		}
		/* end of current token */
		else {
			type = lex_strtot(token_as_string);
			token = lex_tok_new(token_as_string, type);
			if(!array_append(tokens, token)) {
				fprintf(stderr, "lex_strtok2 : call to array_append returned NULL\n");
				return NULL;
			}

			/* alloc new string token */
			alloc_len = 8;
			len = 0;
			token_as_string = malloc(sizeof(char) * (alloc_len + 1));
			if(!token_as_string) {
				fprintf(stderr, "lex_strtok2 : call to malloc returned NULL\n");
				return NULL;
			}
		}
	}

	return tokens;
}

/* Extracts tokens from string without empty chars */
token *lex_strtok(char *next) {
	char *cur;
	int n = 0;
	int size = LEX_TOKENS_INIT_ALLOC;
	token *list = (token *)malloc(sizeof(token) * size);
	token *p = list;

	/* Trimming leading spaces */
	pass_ignore(&next);

	while(!char_is_EOS(*next)) {
		cur = next;
		next_word(&next);

		// TODO: if quotes read => inside string, looking for end of string

		/* Ignoring spaces */
		if(char_is_ignore(*cur)) {
			cur = next;
			next_word(&next);
		}

		/* Adding new token */
		if(char_is_EOW(*cur) && char_is_ignore(*next)) {
			memcpy(p->literal, cur, next - cur);
			p->literal[next - cur] = '\0';
			next_word(&next);
		}
		else {
			memcpy(p->literal, cur, next - cur);
			p->literal[next - cur] = '\0';
		}

		p->type = lex_strtot(p->literal);

		/* Next token */
		n++;
		p++;

		/* Realloc  */
		if(n >= size) {
			size *= LEX_TOKENS_REALLOC_COEF;
			list = (token *)realloc(list, sizeof(token) * size);
			p = list + n;
		}
	}
	/* End of list */
	p->literal[0] = '\0';
	p->type = T_EOT;

	return list;
}

/* Printing 1 token  */
void lex_print_t(token *t) {
	lex_type type;
	const char *lit, *debug;

	type = t->type;
	lit = t->literal;
	debug = lex_ttostr(type);
	printf("%d:%s:%s\n", type, debug, lit);
}

/* Printing all tokens from list */
void lex_print_l(token *l) {
	while(l->type != T_EOT) {
		lex_print_t(l++);
	}
}

bool lex_is_t_basic(lex_type t) {
	return t == T_TYPE_INT || t == T_TYPE_CHAR || t == T_TYPE_VOID;
}

/* Extracts n-1 element from list wich pointer is incremented by n.
 * (ignoring last token as it is a separator).
 * */
token *lex_extract(token **l, int n) {
	int cpt = 0;
	int size = LEX_TOKENS_INIT_ALLOC;
	token *res = (token *)malloc(sizeof(token) * size);
	token *p = res;

	while(n-- > 1) {
		memcpy(p, *l, sizeof(token));
		p++;
		(*l)++;
		cpt++;

		if(cpt >= size) {
			size *= LEX_TOKENS_REALLOC_COEF;
			res = (token *)realloc(res, sizeof(token) * size);
			p = res + cpt;
		}
	}

	/* End of list */
	p->type = T_EOT;

	/* Trimming last token (separator) */
	(*l)++;

	return res;
}

int lex_count(token *l, lex_type type) {
	int n = 0;

	while(l && l->type != type) {
		l++;
		n++;
	}

	return ++n;
}


