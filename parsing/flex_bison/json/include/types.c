#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

type_value *value_new_int(int value) {
	type_value *res = malloc(sizeof(*res));
	res->type = INT;
	res->value = malloc(sizeof(int));
	*(int *)(res->value) = value;
	return res;
}

type_value *value_new_bool(int value) {
	type_value *res = malloc(sizeof(*res));
	res->type = BOOL;
	res->value = malloc(sizeof(int));
	*(int *)(res->value) = value;
	return res;
}


type_value *value_new_double(double value) {
	type_value *res = malloc(sizeof(*res));
	res->type = DOUBLE;
	res->value = malloc(sizeof(double));
	*(double *)(res->value) = value;
	return res;
}

type_value *value_new_string(char *value) {
	type_value *res = malloc(sizeof(*res));
	res->type = STRING;
	res->value = value;
	return res;
}

type_value *value_new_from_array(type_array *a) {
	type_value *res = malloc(sizeof(*res));
	res->type = ARRAY;
	res->value = a;
	return res;
}

type_value *value_new_obj_from_array(type_array *a) {
	type_value *res = malloc(sizeof(*res));
	res->type = OBJ;
	res->value = a;
	return res;
}

type_value *value_new_from_varname(char *name) {
	type_value *res = malloc(sizeof(*res));
	res->type = PTR;
	res->value = name;
	return res;
}

type_prop *prop_new(char *name, type_value *value) { 
	type_prop *res = malloc(sizeof(*res));
	strcpy(res->name, name);
	res->value = value;
	return res;
}

void value_print(void *value) {
	type_value *v = value;

	if(!v) {
		printf("null, ");
		return;
	}

	switch(v->type) {
		case INT:
			printf("%d, ", *(int *)(v->value));
			printf("INT");
			break;
		case BOOL:
			if(*(int *)(v->value)) {
				printf("true, ");
			}
			else {
				printf("false, ");
			}
			printf("BOOL");
			break;
		case DOUBLE:
			printf("%f, ", *(double *)(v->value));
			printf("DOUBLE");
			break;
		case STRING:
			printf("\"%s\", ", (char *)v->value);
			printf("STRING");
			break;
		case ARRAY:
			printf("[ ");
			array_map((type_array *)v->value, value_print);
			printf(" ], ");
			printf("ARRAY");
			break;
		case PTR:
			printf("%s, ", (char *)v->value);
			break;
		case OBJ:
			printf("{\n");
			array_map((type_array *)v->value, prop_print);
			printf("}, ");
			printf("OBJ");
			printf("\n");
			break;
	}
}

void prop_print(void *property) {
	type_prop *p = property;
	printf("\"%s\": ", p->name);
	value_print(p->value);
	printf("\n");
}

type_array *array_new() {
	type_array *res = malloc(sizeof(*res));
	res->size = ARRAY_INIT_SIZE;
	res->n = 0;
	res->data = malloc(sizeof(type_prop *) * res->size);
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
		a = realloc(a, a->size * sizeof(type_prop *));
	}

	a->data[a->n++] = p;

	return a;
}

void array_map(type_array *a, void (*user_print)(void *)) {
	for(size_t i = 0; i < a->n; i++) {
		user_print(a->data[i]);
	}
}

