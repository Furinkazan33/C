#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "str.h"


void str_free(str *s) {
	assert(s && s->start);

	free(s->start);
	free(s);
}

void str_print(str *s) {
	printf("%s\n", s->start);
}
int str_cmp(str *s1, str *s2) {
	return s1->start - s2->start;
}
void str_debug(str *s) {
	if(!s) {
		printf("NULL");
	}
	else {
		printf("s:%c e:%c len:%ld alloc:%ld [%s]\n", \
			*(s->start), *(s->end - 1), s->end - s->start, s->size + 1, s->start);
	}
}


str *str_alloc(size_t init_size) {
	str *res = malloc(sizeof(*res));
	if(!res) {
		perror("str_alloc : alloc failed");
		return NULL;
	}
	if(init_size) {
		res->start = malloc(sizeof(char) * (init_size + 1));
		if(!res->start) {
			perror("str_alloc : content alloc failed");
			free(res);
			return NULL;
		}
	}
	res->size = init_size;
	res->end = res->start;

	return res;
}

str *str_new(char *s, size_t len, size_t init_size) {
	assert(len <= init_size);
	str *res;

 	res = str_alloc(init_size);
	if(!res) {
		perror("");
		return NULL;
	}

	strcpy(res->start, s);
	res->end = res->start + len;

	return res;
}

/* if realloc failed, s is unchanged */
str *str_realloc(str *s, size_t min_size) {
	char *tmp = NULL;
	int len = s->end - s->start; /* to set the end cursor after realloc */

	while(min_size > s->size || (size_t)(s->end - s->start) == s->size) {
		tmp = realloc(s->start, s->size * 2);
		if(!tmp) {
			perror("str_realloc : failed");
			return NULL;
		}
		s->size *= 2;
		s->start = tmp;
		s->end = s->start + len;
	}

	return s;
}

void str_set_end(str *s, int diff) {
	s->end += diff;
}

str *str_copy(str *s) {
	str *res = str_alloc(s->size);
	if(!res) {
		perror("");
		return NULL;
	}
	strcpy(res->start, s->start);
	res->size = s->size;
	res->end = res->start + (s->end - s->start);

	return res;
}


