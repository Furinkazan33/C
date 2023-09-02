#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "str.h"


int str_cmp(void *s1, void *s2) {
	assert(s1);
	assert(s2);
	assert(((str *)s1)->data);
	assert(((str *)s2)->data);

	return strcmp(((str *)s1)->data, ((str *)s2)->data);
}

void str_write(void *s, FILE *file) {
	assert(s);
	str *ss = (str *)s;
	fprintf(file, "[%ld][%ld][%s]\n", ss->alloc_len, strlen(ss->data), ss->data);
}

void str_free(void *s) {
	assert(s);
	free(((str *)s)->data);
	free((str *)s);
}

str *str_new(size_t alloc_len) {
	assert(alloc_len > 0);
	str *res = malloc(sizeof(*res));
	if(!res) {
		perror("str_new : call to malloc returned NULL");
		return NULL;
	}

	res->data = malloc(sizeof(char) * (alloc_len + 1));
	if(!res->data) {
		perror("str_new : call to malloc on data returned NULL");
		return NULL;
	}

	res->alloc_len = alloc_len;

	return res;
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
	strcpy(res->data, string->data);

	return res;
}

str *str_cat(str *string, char *c, size_t len) {
	str *tmp;

	while(strlen(string->data) + len > string->alloc_len) {
		tmp = str_realloc(string, 2);
		if(!tmp) {
			fprintf(stderr, "str_cat : call to str_realloc returned NULL\n");
			return NULL;
		}
		string = tmp;
	}
	strcat(string->data, c);

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

#ifdef DEBUG
	fprintf(stdout, "str_replace : result\n");
	str_write(s, stdout);
#endif
	return s;
}

str *str_set(str *s, char *value, size_t alloc_len) {
	s->alloc_len = alloc_len;
	s->data = value;

#ifdef DEBUG
	fprintf(stdout, "str_set : result\n");
	str_write(s, stdout);
#endif
	return s;
}

str *str_shift(str *string, size_t idx, int n) {
	assert(string);
	assert(idx < strlen(string->data));
	assert((int)(idx + n) >= 0);

	str *tmp;

	/* string to move */
	char *s_shift = malloc(sizeof(char) * (strlen(string->data) - idx + 1));
	strcpy(s_shift, string->data+idx);
#ifdef DEBUG
	fprintf(stdout, "str_shift : [%ld][%d][%s]\n", idx, n, s_shift);
#endif
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

	return string;
}

str *str_remove_char(str *s, size_t idx) {
	assert(s);
	assert(s->data);
	assert(idx < strlen(s->data));

#ifdef DEBUG
	fprintf(stdout, "str_remove_char : char to remove : [%s]\n", s->data+idx);
#endif
	if(!str_shift(s, idx + 1, -1)) {
		fprintf(stderr, "str_remove_char : call to str_shift(str *, %ld, -1) returned NULL\n",
				idx);
		return NULL;
	}

	return s;
}

str *str_insert_char(str *s, size_t idx, char c) {
	assert(s);
	assert(s->data);
	assert(idx < strlen(s->data));

	if(!str_shift(s, idx, 1)) {
		fprintf(stderr, "str_insert_char : call to str_shift(str *, %ld, 1) returned NULL\n",
				idx);
		return NULL;
	}

	s->data[idx] = c;

	return s;
}

/* remove every occurences of escape char */
str *str_normalize(str *s, char escape_char) {
	assert(s);
	assert(s->data);

	size_t i = 0;

	while(s->data[i]) {
		if(s->data[i] == escape_char) {
			str_remove_char(s, i);
		}
		i++;
	}
#ifdef DEBUG
	fprintf(stdout, "str_normalize : result\n");
	str_write(s, stdout);
#endif

	return s;
}

/* escape every chars that match the list by the escape_char */
str *str_escape(str *s, const char *list, char escape_char) {
	assert(s);
	assert(s->data);

	size_t i = 0;

	while(s->data[i]) {
		if(strchr(list, s->data[i])) {
			str_insert_char(s, i, escape_char);
			i++;
		}
		i++;
	}
#ifdef DEBUG
	fprintf(stdout, "str_escape : result\n");
	str_write(s, stdout);
#endif
	return s;
}


