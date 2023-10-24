#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define ERROR_MAX_FN_LEN 64
#define ERROR_MAX_MESS_LEN 128

typedef enum error_context {
	EC_NONE = 0,
	EC_LEXER,
	EC_LEXER_READ_CODE,
	EC_LEXER_STRING,
	EC_LEXER_TOKEN,
	EC_PARSER,
	EC_PARSER_CLEAN,
	EC_PARSER_READ,
	EC_PARSER_READ_INCLUDE,
	EC_PARSER_READ_DEFINE,
	EC_PARSER_READ_VAR,
} error_context;

char *error_context_literal(error_context c) {
	switch(c) {
		case EC_NONE:
			return "NONE";
		case EC_LEXER:
			return "LEXER";
		case EC_LEXER_READ_CODE:
			return "LEXER_READ_CODE";
		case EC_LEXER_STRING:
			return "LEXER_STRING";
		case EC_LEXER_TOKEN:
			return "LEXER_TOKEN";
		case EC_PARSER:
			return "PARSER";
		case EC_PARSER_CLEAN:
			return "PARSER_CLEAN";
		case EC_PARSER_READ:
			return "PARSER_READ";
		case EC_PARSER_READ_INCLUDE:
			return "PARSER_READ_INCLUDE";
		case EC_PARSER_READ_DEFINE:
			return "PARSER_READ_DEFINE";
		case EC_PARSER_READ_VAR:
			return "PARSER_READ_VAR";
	}
}

typedef enum error_type {
	ET_NONE = 0,
	ET_BAD_FORMAT,
	ET_BAD_TYPE,
	ET_BAD_RETURN,
} error_type;

char *error_type_literal(error_type t) {
	switch(t) {
		case ET_NONE:
			return "NONE";
		case ET_BAD_FORMAT:
			return "BAD_FORMAT";
		case ET_BAD_TYPE:
			return "BAD_TYPE";
		case ET_BAD_RETURN:
			return "BAD_RETURN";
	}
}

typedef struct error {
	error_context context;
	error_type type;
	char funcname[ERROR_MAX_FN_LEN];
	char message[ERROR_MAX_MESS_LEN];
	void *p;
} error;


void error_reset(error *e) {
	e->context = EC_NONE;
	e->type = ET_NONE;
	e->funcname[0] = '\0';
	e->message[0] = '\0';
	e->p = NULL;
}

int error_is_set(error *e) {
	if(e->context || e->type || e->funcname || e->message || e->p) {
		return 1;
	}
	return 0;
}

void error_set(error *e, error_context context, error_type type, char *funcname, char *message, void *p) {
	e->context = context;
	e->type = type;
	strncpy(e->funcname, funcname, ERROR_MAX_FN_LEN);
	strncpy(e->message, message, ERROR_MAX_MESS_LEN);
	e->p = p;
}

void error_print(error *e, FILE *file) {
	fprintf(file, "[%s][%s] : %s : %s\n", 
			error_context_literal(e->context), error_type_literal(e->type), 
			e->funcname, e->message);
}

static error err;

void test() {
	error_set(&err, EC_PARSER_READ_VAR, ET_BAD_RETURN, "parser_read_var", "NULL returned", NULL); 
}


int main(void) {
	test();

	if(error_is_set(&err)) {
		error_print(&err, stderr);
	}


	return 0;
}

