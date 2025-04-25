#include <stdio.h>
#include <string.h>
#include "array.h"

array *array_new(size_t size, size_t capacity)	{
	array *res;
	MALLOC(res, sizeof(*res), );
	MALLOC(res->items, size * capacity, free(res); return NULL);
	res->size = size;
	res->count = 0;
	res->capacity = capacity;
	return res;
}

array *array_add(array *a, void *item) {
	if(a->count == a->capacity) {
		REALLOC(a->items, a->size * a->capacity * ARRAY_REALLOC_COEF, return NULL);
		a->capacity *= ARRAY_REALLOC_COEF;
	}

	memcpy((char *)a->items + a->size * a->count++, item, a->size);

	return a;
}

void *array_get(array *a, size_t i) {
	return (char *)a->items + i * a->size;
}

void array_free(array *a) {
	free(a->items);
	free(a);
}

