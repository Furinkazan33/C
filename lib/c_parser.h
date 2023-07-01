#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define _POSIX_C_SOURCE 200809L

/* TODO add dynamic allocations to remove these */
#define LEX_MAX_TOKEN 10000
#define LEX_MAX_TOKEN_LEN 30

#define N_TOKENS_ALLOC 100
#define N_TOKENS_REALLOC_COEF 2

/*
Functions list :
lexer_strtok : get all tokens
lexer_free   : free memory after lexer_strtok call
lexer_strtot : string to type conversion
lexer_ttostr : Type to string conversion
*/
/*
    Token types
*/
typedef enum token_type {
    /* basic types */
    T_TYPE_INT = 0, T_TYPE_CHAR, T_TYPE_VOID, T_TYPE_INT_S, T_TYPE_CHAR_S, T_TYPE_VOID_S,

	T_STRUCT, T_TYPEDEF, T_ENUM,

    /* Specials */
    T_INCLUDE, T_DEFINE, T_RETURN,

	/* Logical */
	T_AND, T_OR, T_EXCL, T_IF, T_ELSE,

    /* single chars */
    T_L_STRIPE, T_R_STRIPE, T_COMMA, T_SEMICOLON, T_EQUAL, T_SPACE, T_Q_MARKS,
    T_L_BRACKET, T_R_BRACKET, T_DOT, T_EOL, T_EOS, T_TAB, T_HYPHEN, T_QUOTE,
	T_L_PAREN, T_R_PAREN, T_SHARP, T_PLUS, T_MINUS, T_STAR, T_SLASH, T_AMPER,

    /* Non-deterministic values */
    T_INT_VALUE, T_NAME,

    /* End of tab index */
    T_EOT,
} lex_type;


typedef struct token {
    char literal[LEX_MAX_TOKEN_LEN];
    lex_type type;
} token;



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
    [T_Q_MARKS]    	=    "\"",
    [T_L_BRACKET]  	=    "{",
    [T_R_BRACKET]  	=    "}",
    [T_DOT]        	=    ".",
    [T_EOL]        	=    "\n",
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
    [T_Q_MARKS]    =    "T_Q_MARKS",
    [T_L_BRACKET]  =    "T_L_BRACKET",
    [T_R_BRACKET]  =    "T_R_BRACKET",
    [T_DOT]        =    "T_DOT",
    [T_EOL]        =    "T_EOL",
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
    [T_INT_VALUE] 	=    "T_INT_VALUE",
    [T_NAME]        =    "T_NAME",

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
    token_type_literal[T_Q_MARKS],
    token_type_literal[T_L_BRACKET],
    token_type_literal[T_R_BRACKET],
    token_type_literal[T_DOT],
    token_type_literal[T_EOL],
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

	/* End of tab */
	token_type_literal[T_EOT]
};


const char *ignore[] = {
	token_type_literal[T_SPACE],
	token_type_literal[T_EOL],
	token_type_literal[T_TAB],

	/* End of tab */
	token_type_literal[T_EOT]
};

/* Char is a digit */
int char_is_int(char c) {
	return c >= 48 && c <= 57;
}

/* Every char is a digit */
int string_is_int(char *s) {
    while(*s != '\0' && char_is_int(*s)) {
		s++;
	}
    return *s == '\0';
}

/* Index is not the last */
int not_EOT_literal(int i) {
	return strcmp(token_type_literal[i], token_type_literal[T_EOT]) != 0;
}

/* Index is not the last */
int not_EOT_debug(int i) {
	return strcmp(token_type_debug[i], token_type_debug[T_EOT]) != 0;
}

/* Index is not the last */
int not_EOT_EOW(int i) {
	return strcmp(end_word[i], token_type_literal[T_EOT]) != 0;
}

/* Index is not the last */
int not_EOT_ignore(int i) {
	return strcmp(ignore[i], token_type_literal[T_EOT]) != 0;
}

/* Defines what an end of word char is */
int char_is_EOW(char c) {
    int i;

    for(i = 0; not_EOT_EOW(i); i++) {
        if(c == end_word[i][0]) {
            return 1;
        }
    }
    return 0;
}

/* Defines what an empty char is  */
int char_is_ignore(char c) {
	int i;

	for(i = 0; not_EOT_ignore(i); i++) {
		if(c == ignore[i][0]) {
			return 1;
		}
	}
	return 0;
}

/* Defines what an end of string char is  */
int char_is_EOS(char c) {
	return c == token_type_literal[T_EOS][0];
}

/* Defines what a valid char is  */
int char_is_valid(char c) {
	return !char_is_EOS(c) && !char_is_EOW(c) && !char_is_ignore(c);
}

/* Get type from string */
lex_type lexer_strtot(char *expression) {
	int i;

    for(i = 0; not_EOT_literal(i); i++) {
        if(strcmp(expression, token_type_literal[i]) == 0) {
            return (lex_type) i;
        }
    }

    if(string_is_int(expression)) {
        return T_INT_VALUE;
    }

    return T_NAME; /* If no token recognize, then it's a name or value */
}

/* Get literal from type */
const char *lexer_ttostr(lex_type type) {
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


/* Extracts tokens from string without empty chars */
token *lex_strtok(char *next) {
	char *cur;
	int n = 0;
	int size = N_TOKENS_ALLOC;
	token *list = (token *)malloc(sizeof(token) * size);
	token *p = list;

	/* Trimming leading spaces */
	pass_ignore(&next);

	while(!char_is_EOS(*next)) {
		cur = next;
		next_word(&next);

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

		p->type = lexer_strtot(p->literal);

		/* Next token */
		n++;
		p++;

		/* Realloc  */
		if(n >= size) {
			size *= N_TOKENS_REALLOC_COEF;
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
    debug = lexer_ttostr(type);
	printf("%d:%s:%s\n", type, debug, lit);
}

/* Printing all tokens from list */
void lex_print_l(token *l) {
	while(l->type != T_EOT) {
        lex_print_t(l++);
	}
}

int lex_is_t_basic(lex_type t) {
	return t == T_TYPE_INT || t == T_TYPE_CHAR || t == T_TYPE_VOID;
}

/* Extracts n-1 element from list wich pointer is incremented by n.
 * (ignoring last token as it is a separator).
 * */
token *lex_extract(token **l, int n) {
	int cpt = 0;
	int size = N_TOKENS_ALLOC;
	token *res = (token *)malloc(sizeof(token) * size);
	token *p = res;

	while(n-- > 1) {
		memcpy(p, *l, sizeof(token));
		p++;
		(*l)++;
		cpt++;

		if(cpt >= size) {
			size *= N_TOKENS_REALLOC_COEF;
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


