#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "btree.h"

#define PC_COMMENT			(1 << 0)
#define PC_COMMENT_LINE		(1 << 1)
#define PC_COMMENT_BLOCK	(1 << 2)
#define PC_BLANK			(1 << 3)
#define PC_NEWLINE			(1 << 4)
#define PC_SPACE			(1 << 5)
#define PC_TAB				(1 << 6)

/* any C statement */
typedef enum c_stat_type {
	CST_NONE = 0,
	CST_INCLUDE,
	CST_DEFINE,
	CST_VAR_DECL,
	CST_VAR_ASSIGN,
	CST_VAR_DECL_ASSIGN,
	CST_FUNC_DECL,
	CST_FUNC_DECL_FULL,
	CST_FUNC_CALL,
	CST_ARRAY,
	CST_RETURN,
	CST_IF,
	CST_ELSE,
	CST_SWITCH,
	CST_CASE,
	CST_DEFAULT,
	CST_LOGICAL,
} c_stat_type;

typedef struct c_statement {
	c_stat_type type; 
	void *data; // cannot be NULL
} c_statement;

typedef enum c_basic_type {
	CBT_NONE = 0,
	CBT_INT, CBT_DOUBLE, CBT_CHAR, CBT_VOID,
	CBT_INT_S, CBT_DOUBLE_S, CBT_CHAR_S, CBT_VOID_S,
} c_basic_type;

typedef enum c_var_type {
	CVT_NONE = 0,
	CVT_VAR,
	CVT_VALUE,
	CVT_FUNC,
	CVT_FUNCALL,
} c_var_type;

typedef struct c_file {
	list *includes;
	list *defines;
	list *variables;
	list *functions;
} c_file;

typedef struct c_include {
	bool local;
	char *filepath;
} c_include;

typedef struct c_value {
	c_basic_type val_type;
	void *data; // pointer to literal value
} c_value;

typedef struct c_variable {
	c_basic_type var_type;
	c_var_type vartype; //if CVT_VALUE, point to c_value
	bool constant;
	size_t n;			// 1 or n for arrays
	size_t mem_alloc;	// number of allocated bytes
	char *name;			// can be NULL for literal values
	void *data; //if not NULL and type==CVT_INT, point to a CVT_LIT_INT ot CVT_INT
} c_variable;

/* TODO: for now, define is a variable */
typedef c_variable c_define;

typedef c_variable c_return_value;
typedef c_variable c_array;

typedef struct c_function_declaration {
	c_basic_type type;
	char *name;
	list *params;		// list of c_variable *
} c_function_declaration;

typedef struct c_function {
	c_function_declaration *declaration;
	list *statements;	// list of c_statement *
} c_function;

typedef struct c_function_call {
	c_function *function;
	list *params;		// list of c_variable *
} c_function_call;

/* A logical expression is represented as a btree of c_logical_value * */
typedef btree c_logical_e;

typedef enum c_log_type { 
	C_LOG_NONE = 0,
	C_LOG_VAR = 0,	// c_variable
	C_LOG_EXPR,		// btree
} c_log_type;

typedef struct c_logical_value {
	c_log_type *type;
	void *data;
} c_logical_value;

typedef struct c_if {
	c_logical_e *condition;
	list *statements;	// list of c_statement *
} c_if;

typedef struct c_else {
	list *statements;	// list of c_statement *
} c_else;


char *parser_type_literal(c_stat_type type);
list *parser_clean(list *tokens, unsigned int options);
list *parser_tokens_to_structs(list *tokens);

void parser_print_statement(void *c_stat, void *file);

