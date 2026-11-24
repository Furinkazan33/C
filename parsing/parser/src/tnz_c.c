#include <stdlib.h>
#include <stdio.h>
#include "tnz_c.h"
#include "tokenizer.h"
#include "macros.h"


const t_block blocks[] = {
	{ .start="#define", .end="\n", .type1=C_LEX_PREPROC, .type2=C_LEX_DEFINE },
	{ .start="#include", .end="\n", .type1=C_LEX_PREPROC, .type2=C_LEX_INCLUDE },
	{ .start="\"", .end="\"", .type1=TNZ_T1_NONE, .type2=C_LEX_VALUE_STRING },
	{ 0 }
};

// TODO: reserved keywords ws separated

const t_reserved reserved[] = {
	{ .content=" ", .type1=C_LEX_WS, .type2=C_LEX_SPACE },
	{ .content="\t", .type1=C_LEX_WS, .type2=C_LEX_TAB },
	{ .content="\n", .type1=C_LEX_WS, .type2=C_LEX_NL },
	{ .content="\b", .type1=C_LEX_WS, .type2=C_LEX_B },
	
	{ .content="#", .type1=C_LEX_OPERATOR, .type2=C_LEX_SHARP },
	{ .content="=", .type1=C_LEX_OPERATOR, .type2=C_LEX_EQUAL },
	{ .content="&&", .type1=C_LEX_OPERATOR, .type2=C_LEX_AND },
	{ .content="&", .type1=C_LEX_OPERATOR, .type2=C_LEX_AMP },
	{ .content="*", .type1=C_LEX_OPERATOR, .type2=C_LEX_STAR },
	{ .content="(", .type1=C_LEX_OPERATOR, .type2=C_LEX_LPAREN },
	{ .content=")", .type1=C_LEX_OPERATOR, .type2=C_LEX_RPAREN },
	{ .content="[", .type1=C_LEX_OPERATOR, .type2=C_LEX_LHOOK },
	{ .content="]", .type1=C_LEX_OPERATOR, .type2=C_LEX_RHOOK },
	{ .content="{", .type1=C_LEX_OPERATOR, .type2=C_LEX_LBRACK },
	{ .content="}", .type1=C_LEX_OPERATOR, .type2=C_LEX_RBRACK },
	{ .content="<", .type1=C_LEX_OPERATOR, .type2=C_LEX_INF },
	{ .content=">", .type1=C_LEX_OPERATOR, .type2=C_LEX_SUP },
	{ .content="switch", .type1=C_LEX_OPERATOR, .type2=C_LEX_SWITCH },
	{ .content="case", .type1=C_LEX_OPERATOR, .type2=C_LEX_CASE },
	{ .content="return", .type1=C_LEX_OPERATOR, .type2=C_LEX_RETURN },
	
	{ .content=",", .type1=C_LEX_SEPARATOR, .type2=C_LEX_COMMA },
	{ .content=".", .type1=C_LEX_SEPARATOR, .type2=C_LEX_DOT },
	{ .content=";", .type1=C_LEX_SEPARATOR, .type2=C_LEX_SEMI },
	{ .content=":", .type1=C_LEX_SEPARATOR, .type2=C_LEX_COLON },
	{ .content="\"", .type1=C_LEX_SEPARATOR, .type2=C_LEX_DQUOTE },
	{ .content="'", .type1=C_LEX_SEPARATOR, .type2=C_LEX_QUOTE },
	
	{ .content="static", .type1=C_LEX_TYPE, .type2=C_LEX_STATIC },
	{ .content="int", .type1=C_LEX_TYPE, .type2=C_LEX_INT },
	{ .content="char", .type1=C_LEX_TYPE, .type2=C_LEX_CHAR },
	{ .content="float", .type1=C_LEX_TYPE, .type2=C_LEX_FLOAT },
	{ .content="const", .type1=C_LEX_TYPE, .type2=C_LEX_CONST },
	{ .content="double", .type1=C_LEX_TYPE, .type2=C_LEX_DOUBLE },
	
	{ 0 }
};

array *tnz_c(char *filepath) {
	char *text = readfile(filepath);
	
	array *tokens = array_new(20, sizeof(token));
	
	if(!tokenize(&tokens, text, blocks, reserved)) {
		LOG_E("error from tokenizer (count=%ld)\n", tokens->count);
		ARRAY_DO(tokens, idx, token *, t, token_fprint(t, stderr); );
		free(text);
		return NULL;
	}
	else {
		#ifdef DEBUG
			LOG_S("Result from tokenizer :\n")
			ARRAY_DO(tokens, idx, token *, t, token_print_formated(t); );
		#endif
		free(text);
		return tokens;
	}
}

void tnz_c_fprint(array *tokens, FILE *out) {
	ARRAY_DO(tokens, idx, token *, t, token_fprint(t, out); );
}

void tnz_c_free(array *tokens) {
	ARRAY_FREE(tokens);
}
