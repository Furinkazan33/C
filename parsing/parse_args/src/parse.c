#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse.h"

char *readline(FILE *f, char *dest, size_t *len) {
	char c;
	*len = 0;
	while((c = fgetc(f)) != EOF) {
			dest[(*len)++] = c;
			if(c == '\n')
					break;
	}
	dest[*len] = '\0';

	return dest;
}

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

bool is_double(char *s, size_t len) {
	bool dot_read = false;
	
	for(size_t i = 0; i < len; i++) {
		if(s[i] == '.') {
			if(dot_read) {
				return false;
			}
			else {
				dot_read = true;
			}
		}
		else if(!isdigit(s[i])) {
			return false;
		}
	}
	
	return dot_read;
}

bool is_integer(char *s, size_t len) {
	for(size_t i = 0; i < len; i++) {
		if(!isdigit(s[i])) {
			return false;
		}
	}
	
	return true;
}

value_type get_type(char *s, size_t len) {
	if(s[0] == '"') {
		return STRING_LIT;
	}
	else if(is_double(s, len)) {
		return DOUBLE;
	}
	else if(is_integer(s, len)) {
		return INTEGER;
	}
	else if(len == 1){
		return CHAR;
	}
	else {
		return STRING;
	}
}

const char *type_to_s(value_type type) {
	if(type == STRING_LIT) {
		return "STRING_LIT";
	}
	else if(type == DOUBLE) {
		return "DOUBLE";
	}
	else if(type == INTEGER) {
		return "INTEGER";
	}
	else if(type == CHAR) {
		return "CHAR";
	}
	else {
		return "STRING";
	}
}

// NULL terminated array
char **tokenize(char *text, size_t len) {
	size_t res_alloc = 20;
	size_t i = 0;
	char **res = malloc(sizeof(char *) * (res_alloc + 1));
	
	for(size_t p = 0; p < len; p++) {
		size_t tok_len = 0;
		char *new_token = NULL;
		
		#ifdef DEBUG
			LOG_S("position :");
			printf("%s", text + p);
		#endif
		
		// skipping space
		if(isspace(text[p])) {
			#ifdef DEBUG
				LOG_S("skipping space\n");
			#endif
			;
		}
		
		// read string lit
		else if(text[p] == '"') {
			#ifdef DEBUG
				LOG_S("reading literal string\n");
			#endif

			for(size_t j = p; j < len; j++) {
				if(tok_len >= 1 && text[j] == '"') {
					break;
				}
				tok_len++;
			}
			
			if(text[p + tok_len] != '"') {
				LOG_E("malformed string\n");
				return NULL;
			}
			
			tok_len++;
		}
		
		// read anything else
		else {
			#ifdef DEBUG
				LOG_S("reading anything\n");
			#endif
			for(size_t j = p; j < len && !isspace(text[j]); j++) {
				tok_len++;
			}
		}
		
		// new token
		if(tok_len > 0) {
			new_token = malloc(sizeof(char) * (tok_len + 1));
			strncpy(new_token, text + p, tok_len);
			new_token[tok_len] = 0;
			p += tok_len;
			
			#ifdef DEBUG
				LOG_S("new_token=");
				printf("[%s]\n", new_token);
			#endif
			
			// realloc array
			if(i == res_alloc) {
				res = realloc(res, sizeof(char *) * 2 * (res_alloc + 1));
				
				if(!res) {
					LOG_E("realloc failed\n");
					return NULL;
				}
				
				res_alloc *= 2;
			}
			
			res[i++] = new_token;
		}
	}

	#ifdef DEBUG
		LOG_S("number of tokens : ");
		printf("[%ld]\n", i);
	#endif
	
	res[i] = NULL;
	
	return res;
}


