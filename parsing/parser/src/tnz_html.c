#include <stdlib.h>
#include <stdio.h>
#include "tnz_html.h"
#include "tokenizer.h"
#include "macros.h"

const t_block blocks[] = {
	{ .start="<!--", .end="--!>", .type1=HTML_COMMENT, .type2=0 },
	{ .start="\"", .end="\"", .type1=HTML_VALUE, .type2=HTML_VALUE_STRING },
	{ 0 }
};

const t_reserved reserved[] = {
	{ .content=" ", .type1=HTML_WS, .type2=HTML_SPACE },
	{ .content="\t", .type1=HTML_WS, .type2=HTML_TAB },
	{ .content="\n", .type1=HTML_WS, .type2=HTML_NL },
	{ .content="\b", .type1=HTML_WS, .type2=HTML_B },
	
	{ .content="/", .type1=HTML_OPERATOR, .type2=HTML_SLASH },
	{ .content="#", .type1=HTML_OPERATOR, .type2=HTML_SHARP },
	{ .content="=", .type1=HTML_OPERATOR, .type2=HTML_EQUAL },
	{ .content="&&", .type1=HTML_OPERATOR, .type2=HTML_AND },
	{ .content="&", .type1=HTML_OPERATOR, .type2=HTML_AMP },
	{ .content="*", .type1=HTML_OPERATOR, .type2=HTML_STAR },
	{ .content="(", .type1=HTML_OPERATOR, .type2=HTML_LPAREN },
	{ .content=")", .type1=HTML_OPERATOR, .type2=HTML_RPAREN },
	{ .content="[", .type1=HTML_OPERATOR, .type2=HTML_LHOOK },
	{ .content="]", .type1=HTML_OPERATOR, .type2=HTML_RHOOK },
	{ .content="{", .type1=HTML_OPERATOR, .type2=HTML_LBRACK },
	{ .content="}", .type1=HTML_OPERATOR, .type2=HTML_RBRACK },
	{ .content="<", .type1=HTML_OPERATOR, .type2=HTML_INF },
	{ .content=">", .type1=HTML_OPERATOR, .type2=HTML_SUP },
	
	{ .content=",", .type1=HTML_SEPARATOR, .type2=HTML_COMMA },
	{ .content=".", .type1=HTML_SEPARATOR, .type2=HTML_DOT },
	{ .content=";", .type1=HTML_SEPARATOR, .type2=HTML_SEMI },
	{ .content=":", .type1=HTML_SEPARATOR, .type2=HTML_COLON },
	
	{ 0 }
};

array *tnz_html(char *filepath) {
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

void tnz_html_fprint(array *tokens, FILE *out) {
	ARRAY_DO(tokens, idx, token *, t, token_fprint(t, out); );
}

void tnz_html_free(array *tokens) {
	ARRAY_FREE(tokens);
}

