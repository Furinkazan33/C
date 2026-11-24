#include "array.h"

typedef enum {
	C_LEX_T1_NONE = 100,
	C_LEX_WS,
	C_LEX_VALUE,
	C_LEX_PREPROC,
	C_LEX_OPERATOR,
	C_LEX_SEPARATOR,
	C_LEX_TYPE,
} type_1;

typedef enum { 
	C_LEX_T2_NONE = 100,
	C_LEX_SPACE, C_LEX_TAB, C_LEX_NL, C_LEX_B, 
	C_LEX_VALUE_STRING,
	C_LEX_DEFINE, C_LEX_INCLUDE, 
	C_LEX_SHARP, C_LEX_EQUAL, C_LEX_AND, C_LEX_AMP, C_LEX_STAR, C_LEX_LPAREN, C_LEX_RPAREN, C_LEX_LHOOK, C_LEX_RHOOK, C_LEX_LBRACK, C_LEX_RBRACK, C_LEX_INF, C_LEX_SUP, C_LEX_SWITCH, C_LEX_CASE, C_LEX_RETURN,
	C_LEX_COMMA, C_LEX_DOT, C_LEX_SEMI, C_LEX_COLON, C_LEX_DQUOTE, C_LEX_QUOTE, 
	C_LEX_STATIC, C_LEX_INT, C_LEX_CHAR, C_LEX_FLOAT, C_LEX_CONST, C_LEX_DOUBLE,
} type_2;

array *tnz_c(char *filepath);

void tnz_c_fprint(array *tokens, FILE *out);
void tnz_c_free(array *tokens);

