#include <stdlib.h>
#include <stdio.h>
#include "tnz_tpl.h"
#include "tokenizer.h"
#include "macros.h"


const t_block blocks_tpl[] = {
	{ 0 }
};

const t_reserved reserved_tpl[] = {
	{ .content=" ", .type1=WS, .type2=SPACE },
	{ .content="\t", .type1=WS, .type2=TAB },
	{ .content="\n", .type1=WS, .type2=NL },
	{ .content="\b", .type1=WS, .type2=B },
	
	{ .content="${", .type1=VAR_START, .type2=0 },
	{ .content="}", .type1=VAR_END, .type2=0 },
	
	{ 0 }
};

array *tnz_tpl(char *filepath) {
	char *text = readfile(filepath);
	
	array *tokens = array_new(20, sizeof(token));
	
	if(!tokenize(&tokens, text, blocks_tpl, reserved_tpl)) {
		LOG_E("error from tokenizer (count=%ld)\n", tokens->count);
		tokens_fprint(tokens, stderr);
		free(text);
		return NULL;
	}
	else {
		#ifdef DEBUG
			LOG_S("Result from tokenizer :\n")
			tokens_print_formated(tokens));
		#endif
		free(text);
		return tokens;
	}
}

