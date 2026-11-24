#include <stdlib.h>
#include <stdio.h>
#include "tnz_default.h"
#include "tokenizer.h"
#include "macros.h"

const t_block blocks[] = {
	{ .start="\"", .end="\"", .type1=LEX_DEFAULT_VALUE, .type2=LEX_DEFAULT_VALUE_STRING },
	{ .start="'", .end="'", .type1=LEX_DEFAULT_VALUE, .type2=LEX_DEFAULT_VALUE_STRING },
	
	{ 0 }
};

const t_reserved reserved[] = {
	{ .content=" ", .type1=LEX_DEFAULT_WS, .type2=LEX_DEFAULT_SPACE },
	{ .content="\t", .type1=LEX_DEFAULT_WS, .type2=LEX_DEFAULT_TAB },
	{ .content="\n", .type1=LEX_DEFAULT_WS, .type2=LEX_DEFAULT_NL },
	{ .content="\b", .type1=LEX_DEFAULT_WS, .type2=LEX_DEFAULT_B },
	
	{ .content="/", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_SLASH },
	{ .content="#", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_SHARP },
	{ .content="=", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_EQUAL },
	{ .content="&&", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_AND },
	{ .content="&", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_AMP },
	{ .content="*", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_STAR },
	{ .content="(", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_LPAREN },
	{ .content=")", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_RPAREN },
	{ .content="[", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_LHOOK },
	{ .content="]", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_RHOOK },
	{ .content="{", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_LBRACK },
	{ .content="}", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_RBRACK },
	{ .content="<", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_INF },
	{ .content=">", .type1=LEX_DEFAULT_OPERATOR, .type2=LEX_DEFAULT_SUP },
	
	{ .content=",", .type1=LEX_DEFAULT_SEPARATOR, .type2=LEX_DEFAULT_COMMA },
	{ .content=".", .type1=LEX_DEFAULT_SEPARATOR, .type2=LEX_DEFAULT_DOT },
	{ .content=";", .type1=LEX_DEFAULT_SEPARATOR, .type2=LEX_DEFAULT_SEMI },
	{ .content=":", .type1=LEX_DEFAULT_SEPARATOR, .type2=LEX_DEFAULT_COLON },

	{ 0 }
};

array *tnz_default(char *filepath) {
	char *text = readfile(filepath);
	
	array *tokens = array_new(20, sizeof(token));
	
	if(!tokenize(&tokens, text, blocks, reserved)) {
		LOG_E("error from tokenizer (count=%ld)\n", tokens->count);
		ARRAY_DO(tokens, idx, token*, t, token_fprint(t, stderr); );
		free(text);
		return NULL;
	}
	else {
		#ifdef DEBUG
			LOG_S("Result from tokenizer :\n")
			ARRAY_DO(tokens, idx, ptr, token_print_formated(ptr));
		#endif
		free(text);
		return tokens;
	}
}

void tnz_default_fprint(array *tokens, FILE *out) {
	ARRAY_DO(tokens, idx, token *, t, token_fprint(t, out); );
}

void tnz_default_free(array *tokens) {
	ARRAY_FREE(tokens);
}

