#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

type_array *array_new() {
	type_array *res = malloc(sizeof(*res));
	res->size = ARRAY_INIT_SIZE;
	res->n = 0;
	res->data = malloc(sizeof(type_balise *) * res->size);
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
		a = realloc(a, a->size * sizeof(type_balise *));
	}

	a->data[a->n++] = p;

	return a;
}

void array_map(type_array *a, void (*user_print)(void *)) {
	for(size_t i = 0; i < a->n; i++) {
		user_print(a->data[i]);
	}
}


type_balise *balise_new(char *name, type_array *data) {
	type_balise *res = malloc(sizeof(*res));
	res->type = T_NORMAL;
	strcpy(res->name, name);
	res->data = data;
	return res;
}

type_balise *balise_new_atom(char *name, char *data) {
	type_balise *res = malloc(sizeof(*res));
	res->type = T_ATOM;
	strcpy(res->name, name);
	res->data = data;
	return res;
}


void print_tab(size_t n) {
	for(size_t i = 0; i < n; i++) {
		printf("\t");
	}
}

void _balise_print(size_t depth, void *balise) {
	type_balise *b = balise;

	print_tab(depth);

	switch(b->type) {
		case T_ATOM:
			printf("<%s>%s</%s>\n", b->name, (char *)b->data, b->name);
			break;
		case T_NORMAL:
			type_array *a = b->data;
			printf("<%s>\n", b->name);
			for(size_t i = 0; i < a->n; i++) {
				_balise_print(depth + 1, a->data[i]);
			}
			print_tab(depth);
			printf("</%s>\n", b->name);
			break;
	}
}

void balise_print(void *balise) {
	_balise_print(0, balise);
}

