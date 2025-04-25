#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

type_array *array_new() {
	type_array *res = malloc(sizeof(*res));
	if(!res) {
		perror("malloc failed");
		fprintf(stderr, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, "failed to alloc array");
		return NULL;
	}
	res->size = ARRAY_INIT_SIZE;
	res->n = 0;
	res->data = malloc(sizeof(void *) * res->size);
	return res;
}

type_array *array_new_from(void *e) {
	type_array *res = array_new();
	array_add(res, e);
	return res;
}

type_array *array_add(type_array *a, void *p) {
	if(a->n >= a->size) {
		a->size *= 2;
		a = realloc(a, a->size * sizeof(void *));
	}

	a->data[a->n++] = p;

	return a;
}

void array_map(type_array *a, void (*user_print)(void *)) {
	for(size_t i = 0; i < a->n; i++) {
		user_print(a->data[i]);
	}
}

void line_print(type_array *l) {
	if(l->n == 0) {
		return;
	}

	for(size_t i = 0; i < l->n - 1; i++) {
		printf("%s;", (char *)l->data[i]);
	}

	printf("%s\n", (char *)l->data[l->n-1]);
}

void lines_print(type_array *l) {
	if(l->n == 0) {
		return;
	}

	for(size_t i = 0; i < l->n; i++) {
		line_print(l->data[i]);
	}
}


