#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "str.h"



str *str_new(size_t alloc_len) {
	assert(alloc_len > 0);
	str *res;

	res = malloc(sizeof(char) * (alloc_len + 1));
	if(!res) {
		perror("str_new : call to malloc returned NULL");
		return NULL;
	}

	res->alloc_len = alloc_len;
	res->n = 0;

	return res;
}

void str_free(str *string) {
	free(string->data);
	free(string);
}


/* If realloc failed, str is unchanged. */
str *str_realloc(str *string, double coef) {
	char *tmp = NULL;

	tmp = realloc(string->data, string->alloc_len * coef + 1);
	if(!tmp) {
		perror("str_realloc : call to realloc returned NULL");
		return NULL;
	}
	string->alloc_len *= coef;
	string->data = tmp;

	return string;
}

str *str_copy(str *string) {
	assert(string);

	str *res = str_new(string->alloc_len);
	if(!res) {
		fprintf(stderr, "str_copy : call to str_new(%ld) returned NULL\n", string->alloc_len);
		return NULL;
	}

	res->alloc_len = string->alloc_len;
	res->n = string->n;
	strcpy(res->data, string->data);

	return res;
}


/* realloc if needed */
str *str_cat(str *string, char *c, size_t len) {
	str *tmp;

	while(string->n + len > string->alloc_len) {
		tmp = str_realloc(string, 2);
		if(!tmp) {
			fprintf(stderr, "str_cat : call to str_realloc returned NULL\n");
			return NULL;
		}
		string = tmp;
	}
	strcat(string->data, c);

	string->n += len;

	return string;
}

/* shift from idx to idx + n. Realloc if needed. */
str *str_shift(str *string, size_t idx, int n) {
	assert(string);
	assert(idx < string->n);
	assert((int)(idx + n) >= 0);

	str *tmp;

	/* string to move */
	char *s_shift = malloc(sizeof(char) * (string->n - idx + 1));
	strcpy(s_shift, string->data+idx);

	/* realloc */
	while(idx + strlen(s_shift) + n > string->alloc_len) {
		tmp = str_realloc(string, 2);
		if(!tmp) {
			fprintf(stderr, "str_shift : call to str_realloc returned NULL\n");
			return NULL;
		}
		string = tmp;
	}

	strcpy(string->data + idx + n, s_shift);

	free(s_shift);

	string->n += n;

	return string;
}

str *str_replace(str *s, char *value) {
	str *tmp;

	/* realloc */
	if(strlen(value) > s->alloc_len) {
		tmp = str_realloc(s, 2);
		if(!tmp) {
			fprintf(stderr, "str_set : call to str_realloc returned NULL\n");
			return NULL;
		}
		s = tmp;
	}
	strcpy(s->data, value);
	s->n = strlen(value);

	return s;
}

str *str_set(str *s, char *value, size_t alloc_len) {
	s->alloc_len = alloc_len;
	s->data = value;
	s->n = strlen(value);

	return s;
}

