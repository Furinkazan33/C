#include <stdbool.h>
#include <string.h>
#include "macros.h"
#include "tokenizer.h"


const t_block t_block_null = { 0 };
const t_reserved t_reserved_null = { 0 };



char *readfile(const char *filename) {
	char *fcontent = NULL;
	int fsize = 0;
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp) {
		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);
		rewind(fp);

		fcontent = malloc(sizeof(char) * fsize);
		fread(fcontent, 1, fsize, fp);

		fclose(fp);
	}

	return fcontent;
}

bool char_is_digit(char c) {
	return c >= '0' && c <= '9';
}

bool string_is_integer(char *s, size_t len) {
	for(size_t i = 0; i < len; i++) {
		if(!char_is_digit(s[i])) {
			return false;
		}
	}
	return true;
}

bool char_is_alpha(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool string_is_alpha(char *s, size_t len) {
	for(size_t i = 0; i < len; i++) {
		if(!char_is_alpha(s[i])) {
			return false;
		}
	}
	return true;
}

bool char_is_alnum(char c) {
	return char_is_digit(c) || char_is_alpha(c);
}

bool string_is_alnum(char *s, size_t len) {
	for(size_t i = 0; i < len; i++) {
		if(!char_is_alnum(s[i])) {
			return false;
		}
	}
	return true;
}

bool char_is_varname_first(char c) {
	return c == '_' || char_is_alpha(c);
}

bool char_is_varname(char c) {
	return c == '_' || char_is_alnum(c);
}

bool string_is_varname(char *s, size_t len) {
	if(!char_is_varname_first(*s)) {
		return false;
	}
	else {
		for(size_t i = 1; i < len; i++) {
			if(!char_is_varname(s[i])) {
				return false;
			}
		}
		return true;
	}
}

void t_reserved_print(const t_reserved *r) {
	printf("[%d][%d][%s]\n",  r->type1, r->type2, r->content);
}

const t_reserved *find(const t_reserved list[], char *search) {
	#ifdef DEBUG_TOKENIZER_2
	LOG_S("looking for [%c%c%c%c%c%c%c%c%c%c...]\n", search[0], search[1], search[2], search[3], search[4], search[5], search[6], search[7], search[8], search[9]);
	#endif
	
	for(int i = 0; memcmp(&list[i], &t_reserved_null, sizeof(t_reserved)) != 0; i++) {
		const t_reserved *r = &list[i];
		
		#ifdef DEBUG_TOKENIZER_2
		LOG_S("t_reserved=");
		t_reserved_print(r);
		#endif
		
		if(!strncmp(r->content, search, strlen(r->content))) {
			#ifdef DEBUG_TOKENIZER_2
			LOG_S("found\n");
			#endif
			return r;
		}
	}
	
	#ifdef DEBUG_TOKENIZER_2
	LOG_S("not found\n");
	#endif
	
	return NULL;
}


void t_block_print(const t_block *b) {
	printf("[%d][%d][%s][%s]\n",  b->type1, b->type2, b->start, b->end);
}

const t_block *read_block(char *text, const t_block *b, int *len) {	
	if(strncmp(b->start, text, strlen(b->start)) != 0) {
		LOG_E("block_start error:[%s][%s]\n", b->start, text);
		return NULL;
	}
	
	text++;
	(*len)++;
	
	for(; strncmp(b->end, text, strlen(b->end)) != 0; text++, (*len)++) {
		if(!*text) {
			LOG_E("block_end error:[%s][%s]\n", b->end, text);
			return NULL;
		}
	}
	
	(*len) += strlen(b->end);
	
	return b;
}

const t_block *read_a_block(char *text, const t_block blocks[], int *len) {
	*len = 0;

	for(int i = 0; memcmp(&blocks[i], &t_block_null, sizeof(t_block)) != 0; i++) {
		const t_block *b = &blocks[i];
		
		#ifdef DEBUG_TOKENIZER
			t_block_print(b);
		#endif
		
		if(!strncmp(b->start, text, strlen(b->start))) {
			const t_block *res = read_block(text, b, len);
			if(!res) {
				LOG_E("error from read_block\n");
			}
			return res;
		}
	}
	return &t_block_null;
}

char *word_cpy(char *word, int len) {
	char *res = malloc(sizeof(char) * (len + 1));
	strncpy(res, word, len);
	res[len] = '\0';
	return res;
}

void token_set(token *t, int type1, int type2, char *content, size_t len) {
	t->type1 = type1;
	t->type2 = type2;
	t->content = word_cpy(content, len);
}

void token_free(void *tok) {
	token *t = tok;
	free(t->content);
}

void tokens_free(array *tokens) {
	ARRAY_DO(tokens, idx, token *, t, { token_free(t); })
	ARRAY_FREE(tokens);
}

void token_print(void *tok) {
	token *t = tok;
	fprintf(stdout, "%s", t->content);
}

void token_fprint(void *tok, FILE *out) {
	token *t = tok;
	fprintf(out, "%s", t->content);
}

void token_print_formated(void *tok) {
	token *t = tok;
	fprintf(stdout, "[%d][%d][%s]", t->type1, t->type2, t->content);
}

void token_fprint_formated(void *tok, FILE *out) {
	token *t = tok;
	fprintf(out, "[%d][%d][%s]",  t->type1, t->type2, t->content);
}

void tokens_print(array *tokens) {
	ARRAY_DO(tokens, idx, token *, t, { token_print(t); })
}

void tokens_fprint(array *tokens, FILE *out) {
	ARRAY_DO(tokens, idx, token *, t, token_fprint(t, out); );
}

void tokens_print_formated(array *tokens) {
	ARRAY_DO(tokens, idx, token *, t, token_print_formated(t); );
}

void tokens_fprint_formated(array *tokens, FILE *out) {
	ARRAY_DO(tokens, idx, token *, t, token_fprint_formated(t, out); );
}


array *tokenize(array **dest, char *text, const t_block blocks[], const t_reserved reserved[]) {
	char *p = text;
	int word_len;
	token tok;
	int t1, t2;
	
	while(p && *p) {
		#ifdef DEBUG_TOKENIZER
			LOG_S("p=[%c]\n", *p);
		#endif
	
		/* 
			find delimited blocks (priority over reserved keywords)
		*/
		const t_block *b = read_a_block(p, blocks, &word_len);
		if(!b) {
			LOG_E("error from read_a_block\n");
			return NULL;
		}
		
		#ifdef DEBUG_TOKENIZER
			LOG_S("word_len=[%d]\n", word_len);
		#endif
		
		if(word_len > 0) {
			token_set(&tok, b->type1, b->type2, p, word_len);
			array_add(*dest, &tok);
			
			#ifdef DEBUG_TOKENIZER
				LOG_S("block token=");
				token_print_formated(token);
				puts("");
			#endif
			
			p += word_len;
		}
		else {
			/* 
			find reserved
			*/
			const t_reserved *found = find(reserved, p);

			if(found) {
				word_len = strlen(found->content);
				
				token_set(&tok, found->type1, found->type2, p, word_len);
				array_add(*dest, &tok);
				
				#ifdef DEBUG_TOKENIZER
					LOG_S("reserved token=");
					token_print_formated(token);
					puts("");
				#endif
				
				p += word_len;
			}
			else {
				/* 
				find everthing else (user specific)
				*/
				word_len = 0;
				while(!found && p && *p) {
					p++;
					word_len++;
					found = find(reserved, p);
				}
				
				t1 = TNZ_T1_NONE;
				t2 = TNZ_T2_NONE;
				
				token_set(&tok, t1, t2,p - word_len, word_len);
				array_add(*dest, &tok);
							
				#ifdef DEBUG_TOKENIZER
					LOG_S("user token=");
					token_print_formated(token);
					puts("");
				#endif
			}
		}
	}
	
	return *dest;
}
