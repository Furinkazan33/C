#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.h"

token T_LT_OP_DOT = { "\0", LT_OPERATOR, LT_OP_DOT };
token T_LT_OP_EQUAL= { "\0", LT_OPERATOR, LT_OP_EQUAL };
token T_LT_OP_PLUS = { "\0", LT_OPERATOR, LT_OP_PLUS };
token T_LT_OP_MINUS = { "\0", LT_OPERATOR, LT_OP_MINUS };
token T_LT_OP_DIV = { "\0", LT_OPERATOR, LT_OP_DIV };
token T_LT_OP_STAR = { "\0", LT_OPERATOR, LT_OP_STAR };
token T_LT_OP_PERCENT = { "\0", LT_OPERATOR, LT_OP_PERCENT };
token T_LT_OP_AND = { "\0", LT_OPERATOR, LT_OP_AND };
token T_LT_OP_OR = { "\0", LT_OPERATOR, LT_OP_OR };
token T_LT_OP_NOT = { "\0", LT_OPERATOR, LT_OP_NOT };
token T_LT_OP_LHOOK = { "\0", LT_OPERATOR, LT_OP_LHOOK };
token T_LT_OP_RHOOK = { "\0", LT_OPERATOR, LT_OP_RHOOK };
token T_LT_OP_LPAREN = { "\0", LT_OPERATOR, LT_OP_LPAREN };
token T_LT_OP_RPAREN = { "\0", LT_OPERATOR, LT_OP_RPAREN };
token T_LT_OP_LSTRIPE = { "\0", LT_OPERATOR, LT_OP_LSTRIPE };
token T_LT_OP_RSTRIPE = { "\0", LT_OPERATOR, LT_OP_RSTRIPE };
token T_LT_OP_LBRACKET = { "\0", LT_OPERATOR, LT_OP_LBRACKET };
token T_LT_OP_RBRACKET = { "\0", LT_OPERATOR, LT_OP_RBRACKET };
token T_LT_OP_COLON = { "\0", LT_OPERATOR, LT_OP_COLON };
token T_LT_OP_SHARP = { "\0", LT_OPERATOR, LT_OP_SHARP };
token T_LT_OP_COMMA = { "\0", LT_OPERATOR, LT_OP_COMMA };
token T_LT_OP_SEMICOLON = { "\0", LT_OPERATOR, LT_OP_SEMICOLON };
token T_LT_OP_QUOTES = { "\0", LT_OPERATOR, LT_OP_QUOTES };
token T_LT_OP_QUOTE = { "\0", LT_OPERATOR, LT_OP_QUOTE };

token T_LT_TYPE = { "\0", LT_TYPE, 0 };
token T_LT_T_VOID = { "\0", LT_TYPE, LT_T_VOID };
token T_LT_T_INT = { "\0", LT_TYPE, LT_T_INT };
token T_LT_T_DOUBLE = { "\0", LT_TYPE, LT_T_DOUBLE };
token T_LT_T_CHAR = { "\0", LT_TYPE, LT_T_CHAR };
token T_LT_T_STRUCT = { "\0", LT_TYPE, LT_T_STRUCT };
token T_LT_T_ENUM = { "\0", LT_TYPE, LT_T_ENUM };

token T_LT_VALUE = { "\0", LT_VALUE, 0 };
token T_LT_V_INT = { "\0", LT_VALUE, LT_V_INT };
token T_LT_V_DOUBLE = { "\0", LT_VALUE, LT_V_DOUBLE };
token T_LT_V_CHAR = { "\0", LT_VALUE, LT_V_CHAR };
token T_LT_V_STRING = { "\0", LT_VALUE, LT_V_STRING };

token T_LT_KW_CONST = { "\0", LT_KEYWORD, LT_KW_CONST };
token T_LT_KW_TYPEDEF = { "\0", LT_KEYWORD, LT_KW_TYPEDEF };
token T_LT_KW_DEFINE = { "\0", LT_KEYWORD, LT_KW_DEFINE };
token T_LT_KW_INCLUDE = { "\0", LT_KEYWORD, LT_KW_INCLUDE };
token T_LT_KW_RETURN = { "\0", LT_KEYWORD, LT_KW_RETURN };
token T_LT_KW_IF = { "\0", LT_KEYWORD, LT_KW_IF };
token T_LT_KW_ELSE = { "\0", LT_KEYWORD, LT_KW_ELSE };
token T_LT_KW_WHILE = { "\0", LT_KEYWORD, LT_KW_WHILE };
token T_LT_KW_SWITCH = { "\0", LT_KEYWORD, LT_KW_SWITCH };
token T_LT_KW_CASE = { "\0", LT_KEYWORD, LT_KW_CASE };
token T_LT_KW_BREAK = { "\0", LT_KEYWORD, LT_KW_BREAK };
token T_LT_KW_DEFAULT = { "\0", LT_KEYWORD, LT_KW_DEFAULT };
token T_LT_KW_IFDEF = { "\0", LT_KEYWORD, LT_KW_IFDEF };
token T_LT_KW_IFNDEF = { "\0", LT_KEYWORD, LT_KW_IFNDEF };
token T_LT_KW_ENDIF = { "\0", LT_KEYWORD, LT_KW_ENDIF };

token T_LT_COMMENT_LINE = { "\0", LT_COMMENT, LT_COMMENT_LINE };
token T_LT_COMMENT_BLOCK = { "\0", LT_COMMENT, LT_COMMENT_BLOCK };

token T_LT_BLANK_NEWLINE = { "\0", LT_BLANK, LT_BLANK_NEWLINE };
token T_LT_BLANK_SPACE = { "\0", LT_BLANK, LT_BLANK_SPACE };
token T_LT_BLANK_TAB = { "\0", LT_BLANK, LT_BLANK_TAB };

token T_LT_NAME = { "\0", LT_NAME, 0 };

list l_value = { &T_LT_VALUE, NULL };
list l_vstring = { &T_LT_V_STRING, NULL };
list l_lparen = { &T_LT_OP_LPAREN, NULL };
list l_sharp = { &T_LT_OP_SHARP, NULL };
list l_lstripe = { &T_LT_OP_LSTRIPE, NULL };
list l_rstripe = { &T_LT_OP_RSTRIPE, NULL };
list l_equal = { &T_LT_OP_EQUAL, NULL };
list l_star = { &T_LT_OP_STAR, NULL };
list l_semi = { &T_LT_OP_SEMICOLON, NULL };
list l_name = { &T_LT_NAME, NULL };

list l_include_local = { &T_LT_KW_INCLUDE, &l_vstring };
list L_INCLUDE_local = { &T_LT_OP_SHARP, &l_include_local };

list l_include_name_stripe = { &T_LT_NAME, &l_rstripe };
list l_include_dot_name_stripe = { &T_LT_OP_DOT, &l_include_name_stripe };
list l_include_name_dot_name_stripe = { &T_LT_NAME, &l_include_dot_name_stripe };
list l_include_strip = { &T_LT_OP_LSTRIPE, &l_include_name_dot_name_stripe };
list l_include_global = { &T_LT_KW_INCLUDE, &l_include_strip };
list L_INCLUDE_global = { &T_LT_OP_SHARP, &l_include_global };

list l_define = { &T_LT_KW_DEFINE, &l_name };
list L_DEFINE = { &T_LT_OP_SHARP, &l_define };

list l_name_semi = { &T_LT_NAME, &l_semi };
list l_star_name_semi = { &T_LT_OP_STAR, &l_name_semi };

list L_VAR_DECL = { &T_LT_TYPE, &l_name_semi };
list L_VAR_DECL_PTR = { &T_LT_TYPE, &l_star_name_semi };

list L_COMMENT_line = { &T_LT_COMMENT_LINE, NULL };
list L_COMMENT_block = { &T_LT_COMMENT_BLOCK, NULL };

list L_IF = { &T_LT_KW_IF, &l_lparen };

list l_name_equal = { &T_LT_NAME, &l_equal };
list l_star_name_equal = { &T_LT_OP_STAR, &l_name_equal };
list L_VAR_DECL_ASS = { &T_LT_TYPE, &l_name_equal };
list L_VAR_DECL_ASS_PTR = { &T_LT_TYPE, &l_star_name_equal };


char *parser_CST_literal(c_stat_type type) {
	switch(type) {
		case CST_NONE:
			return "CST_NONE";
		case CST_COMMENT:
			return "CST_COMMENT";
		case CST_INCLUDE:
			return "CST_INCLUDE";
		case CST_DEFINE:
			return "CST_DEFINE";
		case CST_VAR_DECL:
			return "CST_VAR_DECL";
		case CST_VAR_ASSIGN:
			return "CST_VAR_ASSIGN";
		case CST_VAR_DECL_ASSIGN:
			return "CST_VAR_DECL_ASSIGN";
		case CST_FUNC_DECL:
			return "CST_FUNC_DECL";
		case CST_FUNC_DECL_FULL:
			return "CST_FUNC_DECL_FULL";
		case CST_FUNC_CALL:
			return "CST_FUNC_CALL";
		case CST_ARRAY:
			return "CST_ARRAY";
		case CST_RETURN:
			return "CST_RETURN";
		case CST_IF:
			return "CST_IF";
		case CST_ELSE:
			return "CST_ELSE";
		case CST_SWITCH:
			return "CST_SWITCH";
		case CST_CASE:
			return "CST_CASE";
		case CST_DEFAULT:
			return "CST_DEFAULT";
		case CST_LOGICAL:
			return "CST_LOGICAL";
		default:
			fprintf(stderr, "parser_CST_literal : unknown type : [%d]\n", type);
			return NULL;
	}
}

char *parser_CVT_literal(c_var_type type) {
	switch(type) {
		case CVT_NONE:
			return "CVT_NONE";
		case CVT_VAR:
			return "CVT_VAR";
		case CVT_VALUE:
			return "CVT_VALUE";
		case CVT_FUNC:
			return "CVT_FUNC";
		case CVT_FUNCALL:
			return "CVT_FUNCALL";
		default:
			fprintf(stderr, "parser_CVT_literal : unknown type : [%d]\n", type);
			return NULL;
	}
}

char *parser_CCT_literal(c_com_type type) {
	switch(type) {
		case CCT_NONE:
			return "CCT_NONE";
		case CCT_LINE:
			return "CCT_LINE";
		case CCT_BLOCK:
			return "CCT_BLOCK";
		default:
			fprintf(stderr, "parser_CCT_literal : unknown type : [%d]\n", type);
			return NULL;
	}
}

char *parser_CIT_literal(c_inc_type type) {
	switch(type) {
		case CIT_NONE:
			return "CIT_NONE";
		case CIT_LOCAL:
			return "CIT_LOCAL";
		case CIT_GLOBAL:
			return "CIT_GLOBAL";
		default:
			fprintf(stderr, "parser_CIT_literal : unknown type : [%d]\n", type);
			return NULL;
	}
}

bool _to_clean(unsigned int options, lex_token_type type, int type2) {
	if(options & PC_COMMENT) {
		if(type == LT_COMMENT) {
			return true;
		}
	}
	if(options & PC_COMMENT_LINE) {
		if(type == LT_COMMENT && type2 == LT_COMMENT_LINE) {
			return true;
		}
	}
	if(options & PC_COMMENT_BLOCK) {
		if(type == LT_COMMENT && type2 == LT_COMMENT_BLOCK) {
			return true;
		}
	}
	if(options & PC_BLANK) {
		if(type == LT_BLANK) {
			return true;
		}
	}
	if(options & PC_NEWLINE) {
		if(type == LT_BLANK && type2 == LT_BLANK_NEWLINE) {
			return true;
		}
	}
	if(options & PC_TAB) {
		if(type == LT_BLANK && type2 == LT_BLANK_TAB) {
			return true;
		}
	}
	if(options & PC_SPACE) {
		if(type == LT_BLANK && type2 == LT_BLANK_SPACE) {
			return true;
		}
	}
	return false;
}

/* remove corresponding tokens from list */
void parser_clean(list *l, unsigned int options) {
	assert(l);

	token *t = NULL;
	list *prev = NULL;
	list *next = NULL;

	for(list *p = l; p; ) { 
		t = p->data;
		next = p->next;

		if(_to_clean(options, t->type, t->type2)) {

			/* removing elt pointed by l => change pointer to the next  */
			if(p == l) {
				list_next(&l);
			}
			else {
				prev->next = next;
			}
			LIST_FREE(p, lex_token_free);
		}
		else {
			prev = p;
		}
		p = next;
	}
}

bool parser_same_types(void *token1, void *token2) {
	token *t1 = token1;
	token *t2 = token2;

	/* do not compare type2 for these */
	if(t1->type == LT_TYPE || t1->type == LT_VALUE || t1->type == LT_NAME || t1->type == LT_NONE) {
		return t1->type == t2->type;
	}

	return t1->type == t2->type && t1->type2 == t2->type2;
}


c_include *parser_new_include(c_inc_type type, char *filepath) {
	c_include *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_include : call to malloc returned NULL\n");
		return NULL;
	}
	res->type = type;
	res->filepath = filepath;

	return res;
}

/* when a value is read */
c_value *parser_new_c_value_int(int value) {
	c_value *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_c_value_int : call to malloc returned NULL\n");
		return NULL;
	}
	res->type = LT_V_INT;
	res->value_int = value;

	return res;
}

/* when a value is read */
c_value *parser_new_c_value_double(double value) {
	c_value *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_c_value_double : call to malloc returned NULL\n");
		return NULL;
	}
	res->type = LT_V_DOUBLE;
	res->value_double = value;

	return res;
}

/* when a value is read */
c_value *parser_new_c_value_char(char value) {
	c_value *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_c_value_char : call to malloc returned NULL\n");
		return NULL;
	}
	res->type = LT_V_CHAR;
	res->value_char = value;

	return res;
}

//TODO
//c_variable *parser_new_var_ptr(bool constant, char *name, lex_token_type type) {
//&}

/* when variable is declared */
c_variable *parser_new_var(bool constant, bool ptr, char *name, lex_type type) {
	c_variable *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_var : call to malloc(1) returned NULL\n");
		return NULL;
	}

	res->type = type;
	res->var_type = CVT_NONE; // unknown for now
	res->ptr = ptr;
	res->constant = constant;
	res->name = malloc(sizeof(char) * (strlen(name) + 1));
	if(!res->name) {
		fprintf(stderr, "parser_new_var : call to malloc(2) returned NULL\n");
		return NULL;
	}
	strcpy(res->name, name);
	res->data = NULL;

	return res;
}

/* when variable is assigned */
void parser_var_assign_value(c_variable *var, c_value *val) {
	var->var_type = CVT_VALUE;
	var->data = val;
}

/* when variable is assigned */
void parser_var_assign_variable(c_variable *var, c_variable *val) {
	var->var_type = CVT_VAR;
	var->data = val;
}

/* when variable is assigned */
void parser_var_assign_funcall(c_variable *var, c_function_call *val) {
	var->var_type = CVT_FUNCALL;
	var->data = val;
}

/* when variable is assigned */
void parser_var_assign_function(c_variable *var, c_function *val) {
	var->var_type = CVT_FUNC;
	var->data = val;
}

/* when variable is declared and assign */
c_variable *parser_new_var_int(bool constant, char *name, int value) {
	c_variable *res = parser_new_var(constant, false, name, LT_T_INT);
	if(!res) {
		fprintf(stderr, "parser_new_var_int : call to parser_new_var returned NULL\n");
		return NULL;
	}

	c_value *cv = parser_new_c_value_int(value);
	if(!cv) {
		fprintf(stderr, "parser_new_var_int : call to parser_new_c_value_int returned NULL\n");
		return NULL;
	}

	parser_var_assign_value(res, cv);

	return res;
}

/* when variable is declared and assign */
c_variable *parser_new_var_double(bool constant, char *name, double value) {
	c_variable *res = parser_new_var(constant, false, name, LT_T_DOUBLE);
	if(!res) {
		fprintf(stderr, "parser_new_var_double : call to parser_new_var returned NULL\n");
		return NULL;
	}

	c_value *cv = parser_new_c_value_double(value);
	if(!cv) {
		fprintf(stderr, "parser_new_var_double : call to parser_new_c_value_double returned NULL\n");
		return NULL;
	}

	parser_var_assign_value(res, cv);

	return res;
}

/* when variable is declared and assign */
c_variable *parser_new_var_char(bool constant, char *name, char value) {
	c_variable *res = parser_new_var(constant, false, name, LT_T_CHAR);
	if(!res) {
		fprintf(stderr, "parser_new_var_char : call to parser_new_var returned NULL\n");
		return NULL;
	}

	c_value *cv = parser_new_c_value_char(value);
	if(!cv) {
		fprintf(stderr, "parser_new_var_char : call to parser_new_c_value_char returned NULL\n");
		return NULL;
	}

	parser_var_assign_value(res, cv);

	return res;
}

/* when variable is declared and assign 
 * value is copied without starting '"' and ending '"' 
 */
c_variable *parser_new_var_string(bool constant, char *name, char *value) {
	c_variable *res = parser_new_var(constant, true, name, LT_T_CHAR);
	if(!res) {
		fprintf(stderr, "parser_new_var_string : call to parser_new_var returned NULL\n");
		return NULL;
	}

	res->data = malloc(sizeof(char) * (strlen(value) + 1));
	if(!res->data) {
		fprintf(stderr, "parser_new_var_string : call to malloc returned NULL\n");
		return NULL;
	}

	strcpy(res->data, value);

	return res;
}

c_define *parser_new_define_int(char *name, int value) {
	return (c_define *)parser_new_var_int(true, name, value);
}

c_define *parser_new_define_double(char *name, double value) {
	return (c_define *)parser_new_var_double(true, name, value);
}

c_define *parser_new_define_char(char *name, char value) {
	return (c_define *)parser_new_var_char(true, name, value);
}

/* value is copied */
c_define *parser_new_define_string(char *name, char *value) {
	return (c_define *)parser_new_var_string(true, name, value);
}

/* comment is copied without delimiters */
c_comment *parser_new_comment_line(char *content) {
	size_t len;
	c_comment *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_comment_line : call to malloc(1) returned NULL\n");
		return NULL;
	}

	res->type = CCT_LINE;
	len = strlen(content) - 2;

	/* same alloc as read content */
	res->content = malloc(sizeof(char) * (len + 1));
	if(!res) {
		fprintf(stderr, "parser_new_comment_line : call to malloc(2) returned NULL\n");
		return NULL;
	}
	strcpy(res->content, content + 2);
	res->content[len] = '\0';

	return res;
}

/* comment is copied without delimiters */
c_comment *parser_new_comment_block(char *content) {
	size_t len;
	c_comment *res = malloc(sizeof(*res));
	if(!res) {
		fprintf(stderr, "parser_new_comment_block : call to malloc(1) returned NULL\n");
		return NULL;
	}

	res->type = CCT_BLOCK;
	len = strlen(content) - 4;

	/* same alloc as read content */
	res->content = malloc(sizeof(char) * (len + 1));
	if(!res) {
		fprintf(stderr, "parser_new_comment_block : call to malloc(2) returned NULL\n");
		return NULL;
	}
	strcpy(res->content, content + 2);
	res->content[len] = '\0';

	return res;
}

c_comment *parser_read_comment_line(list **p) {
	assert(p);
	assert(*p);

	token *t = (*p)->data;
	c_comment *res = parser_new_comment_line(t->literal);
	if(!res) {
		fprintf(stderr, "parser_read_comment_line : call to parser_new_comment_line returned NULL\n");
		return NULL;
	}
	return res;
}

c_comment *parser_read_comment_block(list **p) {
	assert(p);
	assert(*p);

	token *t = (*p)->data;
	c_comment *res = parser_new_comment_block(t->literal);
	if(!res) {
		fprintf(stderr, "parser_read_comment_block : call to parser_new_comment_block returned NULL\n");
		return NULL;
	}
	return res;
}

c_include *parser_read_include_local(list **p0) {
	assert(*p0);

	token *t0;
	c_include *res = NULL;

	list_next(p0);
	list_next(p0);
	t0 = (*p0)->data;
	list_next(p0);

	size_t len = strlen(t0->literal) - 2;
	char *filepath = malloc(sizeof(char) * (len + 1));
	if(!filepath) {
		fprintf(stderr, "parser_read_include_local : call to malloc(0) returned NULL\n");
		return NULL;
	}
	strcpy(filepath, t0->literal + 1);
	filepath[len] = '\0';

	res = parser_new_include(CIT_LOCAL, filepath);
	if(!res) {
		fprintf(stderr, "parser_read_include_local : call to parser_new_include returned NULL\n");
		return NULL;
	}

	return res;
}

c_include *parser_read_include_global(list **p0) {
	assert(*p0);

	token *t0, *t1, *t2;
	c_include *res = NULL;

	list_next(p0);
	list_next(p0);
	list_next(p0);

	t0 = (*p0)->data;
	list_next(p0);
	t1 = (*p0)->data;
	list_next(p0);
	t2 = (*p0)->data;
	list_next(p0);

	char *filepath = malloc(sizeof(char) * (strlen(t0->literal) + strlen(t1->literal) + strlen(t2->literal) + 1));
	if(!filepath) {
		fprintf(stderr, "parser_read_include_global : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(filepath, t0->literal); // filepath
	strcat(filepath, t1->literal); // dot
	strcat(filepath, t2->literal); // extension
	filepath[strlen(filepath)] = '\0';

	res = parser_new_include(CIT_GLOBAL, filepath);
	if(!res) {
		fprintf(stderr, "parser_read_include_global : call to parser_new_include returned NULL\n");
		return NULL;
	}

	return res;
}

c_define *parser_read_define_int(list **p0) {
	assert(*p0);

	token *t0;
	c_define *res = NULL;

	list_next(p0); //#
	list_next(p0); //define
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_define : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname
	t0 = (*p0)->data;

	res = parser_new_define_int(varname, atoi(t0->literal));
	if(!res) {
		fprintf(stderr, "parser_read_define_int : call to parser_new_define_int returned NULL\n");
		return NULL;
	}

	return res;
}

c_define *parser_read_define_double(list **p0) {
	assert(*p0);

	token *t0;
	c_define *res = NULL;

	list_next(p0); //#
	list_next(p0); //define
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_define_double : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname
	t0 = (*p0)->data;

	res = parser_new_define_double(varname, atof(t0->literal));

	if(!res) {
		fprintf(stderr, "parser_read_define_double : call to parser_new_define_double returned NULL\n");
		return NULL;
	}

	return res;
}

c_define *parser_read_define_char(list **p0) {
	assert(*p0);

	token *t0;
	c_define *res = NULL;

	list_next(p0); //# => define
	list_next(p0); //define => varname
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_define_char : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname => value
	t0 = (*p0)->data;

	res = parser_new_define_char(varname, t0->literal[1]);

	if(!res) {
		fprintf(stderr, "parser_read_define_char : call to parser_new_define_char returned NULL\n");
		return NULL;
	}

	return res;
}

c_define *parser_read_define_string(list **p0) {
	assert(*p0);

	token *t0;
	c_define *res = NULL;

	list_next(p0); //#
	list_next(p0); //define
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_define_string : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname
	t0 = (*p0)->data;

	size_t len = strlen(t0->literal) - 2;
	char *value_s = malloc(sizeof(char) * (len + 1));
	strcpy(value_s, t0->literal + 1);
	value_s[len] = '\0';
	res = parser_new_define_string(varname, value_s);

	if(!res) {
		fprintf(stderr, "parser_read_define_string : call to parser_new_define_string returned NULL\n");
		return NULL;
	}

	return res;
}

c_define *parser_read_define(list **p, lex_value type) {
	assert(*p);

	switch(type) {
		case LT_V_INT:
			return parser_read_define_int(p);
		case LT_V_DOUBLE:
			return parser_read_define_double(p);
		case LT_V_CHAR:
			return parser_read_define_char(p);
		case LT_V_STRING:
			return parser_read_define_string(p);
		case LT_V_NONE:
		default:
			fprintf(stderr, "parser_read_define : type error : %s\n", lex_LV_literal(type));
			return NULL;
	}
}

//TODO: if first token is 'const'
c_variable *parser_read_var_decl(list **p0, lex_type type) {
	assert(*p0);

	token *t0;
	c_variable *res = NULL;

	list_next(p0); // type => varname
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_var_decl : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname => ';'

	res = parser_new_var(false, false, varname, type);
	if(!res) {
		fprintf(stderr, "parser_read_var_decl : call to parser_new_var returned NULL\n");
		return NULL;
	}

	return res;
}

//TODO: if first token is 'const'
c_variable *parser_read_var_decl_ptr(list **p0, lex_type type) {
	assert(*p0);

	token *t0;
	c_variable *res = NULL;

	list_next(p0); // type => '*'
	list_next(p0); // '*' => varname
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_var_decl_ptr : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); //varname => ';'

	res = parser_new_var(false, true, varname, type);
	if(!res) {
		fprintf(stderr, "parser_read_var_decl_ptr : call to parser_new_var returned NULL\n");
		return NULL;
	}

	return res;
}

//TODO: if first token is 'const'
c_variable *parser_read_var_decl_ass(list **p0, lex_type type) {
	assert(*p0);

	token *t0, *t1;
	c_variable *res = NULL;

	list_next(p0); // type => varname
	t0 = (*p0)->data;

	char *varname = malloc(sizeof(char) * (strlen(t0->literal) + 1));
	if(!varname) {
		fprintf(stderr, "parser_read_var_decl_ass : call to malloc returned NULL\n");
		return NULL;
	}
	strcpy(varname, t0->literal);

	list_next(p0); // varname => '='
	list_next(p0); // '=' => value
	t1 = (*p0)->data; //value
	list_next(p0); // value => ';'

	switch(type) {
		case LT_T_INT:
			res = parser_new_var_int(false, varname, atoi(t1->literal));
			break;
		case LT_T_DOUBLE:
			res = parser_new_var_double(false, varname, atof(t1->literal));
			break;
		case LT_T_CHAR:
			res = parser_new_var_char(false, varname, t1->literal[1]);
			break;
		default:
			fprintf(stderr, "parser_read_var_decl_ass : forbidden type\n");
			return NULL;
	}

	if(!res) {
		fprintf(stderr, "parser_read_var_decl_ass : call to parser_new_var returned NULL\n");
		return NULL;
	}

	return res;
}



void c_comment_print_stderr(c_comment *c) {
	fprintf(stderr, "[%s][%s]\n", parser_CCT_literal(c->type), c->content);
}

void c_include_print_stderr(c_include *c) {
	fprintf(stderr, "[%s][%s]\n", parser_CIT_literal(c->type), c->filepath);
}

void c_variable_print_stderr(c_variable *c) {
	fprintf(stderr, "[%s][%s][%d][%s]", 
			lex_LT_literal(c->type), parser_CVT_literal(c->var_type), c->constant, c->name);

	switch(c->var_type) {
		case CVT_NONE:
			fprintf(stderr, "\n");
			break;
		case CVT_VALUE:
			switch(c->type) {
				case LT_T_INT:
					fprintf(stderr, "[%d]\n", ((c_value *)c->data)->value_int);
					break;
				case LT_T_DOUBLE:
					fprintf(stderr, "[%f]\n", ((c_value *)c->data)->value_double);
					break;
				case LT_T_CHAR:
					if(c->ptr) {
						fprintf(stderr, "[%s]\n", (char *)c->data);
					}
					else {
						fprintf(stderr, "[%c]\n", ((c_value *)c->data)->value_char);
					}
					break;
				default:
					fprintf(stderr, ", type error : [%d]\n", c->type);
					break;
			}
			break;
		case CVT_VAR:
			//TODO
			printf("\n");
			break;
		case CVT_FUNC:
			//TODO
			break;
		case CVT_FUNCALL:
			//TODO
			break;
		default:
			//TODO
			fprintf(stderr, ", type error : [%d]\n", c->var_type);
			break;
	}
}

void c_comment_print(c_comment *c) {
	printf("[%s][%s]\n", parser_CCT_literal(c->type), c->content);
}

void c_include_print(c_include *c) {
	printf("[%s][%s]\n", parser_CIT_literal(c->type), c->filepath);
}

void c_variable_print(c_variable *c) {
	printf("[%s][%s][%d][%s]", 
			lex_LT_literal(c->type), parser_CVT_literal(c->var_type), c->constant, c->name);

	switch(c->var_type) {
		case CVT_NONE:
			printf("\n");
			break;
		case CVT_VALUE:
			switch(c->type) {
				case LT_T_INT:
					printf("[%d]\n", ((c_value *)c->data)->value_int);
					break;
				case LT_T_DOUBLE:
					printf("[%f]\n", ((c_value *)c->data)->value_double);
					break;
				case LT_T_CHAR:
					if(c->ptr) {
						printf("[%s]\n", (char *)c->data);
					}
					else {
						printf("[%c]\n", ((c_value *)c->data)->value_char);
					}
					break;
				default:
					fprintf(stderr, ", type error : [%d]\n", c->type);
					break;
			}
			break;
		case CVT_VAR:
			//TODO
			printf("\n");
			break;
		case CVT_FUNC:
			//TODO
			break;
		case CVT_FUNCALL:
			//TODO
			break;
		default:
			//TODO
			fprintf(stderr, ", type error : [%d]\n", c->var_type);
			break;
	}
}

list *parser_tokens_to_structs(list *tokens) {
	list *res = NULL;
	list *p = tokens;
	c_statement *s;
	bool found;

	/* for errors detection */
	list *e, *max_e;
	c_stat_type error_type;
	token *t, *error_token;

	while(p) {
		s = malloc(sizeof(c_statement));
		if(!s) {
			fprintf(stderr, "parser_tokens_to_structs : call to malloc returned NULL\n");
			return NULL;
		}

		t = p->data;
		max_e = p;
		found = false;

		if(!found && list_equal(p, &L_COMMENT_line, parser_same_types, &e)) {
			found = true;
			s->type = CST_COMMENT;
			s->data = parser_read_comment_line(&p);
		}
		if(e > max_e) { max_e = e; error_type = CST_COMMENT; }

		if(!found && list_equal(p, &L_COMMENT_block, parser_same_types, &e)) {
			found = true;
			s->type = CST_COMMENT;
			s->data = parser_read_comment_block(&p);
		}
		if(e > max_e) { max_e = e; error_type = CST_COMMENT; }

		if(!found && list_equal(p, &L_INCLUDE_global, parser_same_types, &e)) {
			found = true;
			s->type = CST_INCLUDE;
			s->data = parser_read_include_global(&p);
		}
		if(e > max_e) { max_e = e; error_type = CST_INCLUDE; }

		if(!found && list_equal(p, &L_INCLUDE_local, parser_same_types, &e)) {
			found = true;
			s->type = CST_INCLUDE;
			s->data = parser_read_include_local(&p);
		}
		if(e > max_e) { max_e = e; error_type = CST_INCLUDE; }

		if(!found && list_equal(p, &L_DEFINE, parser_same_types, &e)) {
			found = true;
			s->type = CST_DEFINE;
			t = p->next->next->next->data;
			s->data = parser_read_define(&p, t->type2);
		}
		if(e > max_e) { max_e = e; error_type = CST_DEFINE; }

		if(!found && list_equal(p, &L_VAR_DECL, parser_same_types, &e)) {
			found = true;
			s->type = CST_VAR_DECL;
			s->data = parser_read_var_decl(&p, t->type2);
		}
		if(e > max_e) { max_e = e; error_type = CST_VAR_DECL; }

		if(!found && list_equal(p, &L_VAR_DECL_PTR, parser_same_types, &e)) {
			found = true;
			s->type = CST_VAR_DECL;
			s->data = parser_read_var_decl_ptr(&p, t->type2);
		}
		if(e > max_e) { max_e = e; error_type = CST_VAR_DECL; }

		if(!found && list_equal(p, &L_VAR_DECL_ASS, parser_same_types, &e)) {
			found = true;
			s->type = CST_VAR_DECL_ASSIGN;
			s->data = parser_read_var_decl_ass(&p, t->type2);
		}
		if(e > max_e) { max_e = e; error_type = CST_VAR_DECL_ASSIGN; }




		if(!found) {
			error_token = max_e->data;
			fprintf(stderr, "parser_tokens_to_structs : error on %s statement near token [%s]\n",
					parser_CST_literal(error_type), error_token->literal);
			fprintf(stderr, "parser_tokens_to_structs : %s %s %s\n", 
					((token *)e->data)->literal,
					((token *)e->next->data)->literal,
					((token *)e->next->next->data)->literal);

			return NULL;
		}

		/*
		 * TODO:
		   case CST_VAR_ASSIGN: 
		   case CST_VAR_DECL_ASSIGN: 
		   case CST_FUNC_DECL: 
		   case CST_FUNC_DECL_FULL: 
		   case CST_FUNC_CALL: 
		   case CST_ARRAY: 
		   case CST_RETURN: 
		   case CST_IF: 
		   case CST_ELSE: 
		   case CST_SWITCH: 
		   case CST_CASE: 
		   case CST_DEFAULT: 
		   case CST_LOGICAL: 
		   case CST_NONE:
		   */

#ifdef DEBUG
		parser_print_statement_stderr(s);
#endif
		/* first statement => list creation */
		if(!res) {
			res = list_new(s);
			if(!res) {
				fprintf(stderr, "parser_tokens_to_structs : call to list_new returned NULL\n");
				return NULL;
			}
		}
		/* adding statement to the list */
		else {
			res = list_insert_after(res, s);
			if(!res) {
				fprintf(stderr, "parser_tokens_to_structs : call to list_insert_after return NULL\n");
				return NULL;
			}
		}
		list_next(&p);
	}

	return res;
}

int parser_print_statement_stderr(void *stat) {
	c_statement *s = stat;
	c_stat_type type = s->type;

	fprintf(stderr, "parser_print_statement : %s : ", parser_CST_literal(type));

	switch(type) {
		case CST_NONE:
			fprintf(stderr, "\n");
			break;
		case CST_COMMENT:
			c_comment_print_stderr(s->data);
			break;
		case CST_INCLUDE:
			c_include_print_stderr(s->data);
			break;
		case CST_DEFINE:
			c_variable_print_stderr(s->data);
			break;
		case CST_VAR_DECL:
		case CST_VAR_ASSIGN:
		case CST_VAR_DECL_ASSIGN:
			c_variable_print_stderr(s->data);
			break;
		default:
			fprintf(stderr, "unknown type : [%d]\n", type);
			return 0;
	}
	return 1;
}

int parser_print_statement(void *stat) {
	c_statement *s = stat;
	c_stat_type type = s->type;

	printf("parser_print_statement : %s : ", parser_CST_literal(type));

	switch(type) {
		case CST_NONE:
			printf("\n");
			break;
		case CST_COMMENT:
			c_comment_print(s->data);
			break;
		case CST_INCLUDE:
			c_include_print(s->data);
			break;
		case CST_DEFINE:
			c_variable_print(s->data);
			break;
		case CST_VAR_DECL:
		case CST_VAR_ASSIGN:
		case CST_VAR_DECL_ASSIGN:
			c_variable_print(s->data);
			break;
		default:
			printf("unknown type : [%d]\n", type);
			return 0;
	}
	return 1;
}


