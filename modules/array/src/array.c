#include <string.h>
#include "array.h"

array *array_new(size_t capacity, size_t size_of_item) {
	array *res = malloc(sizeof(*res));
	res->count = 0;
	res->size_of_item = size_of_item;
	res->items = malloc(size_of_item * capacity);
	res->capacity = capacity;

	return res;
}

void array_free(void *a) {
	array *ar = a;

	free(ar->items);
	free(ar);
}

array *array_append(void *a, void *item) {
	array *ar = a;

	if(ar->count == ar->capacity) {
		if(ar->capacity == 0) {
			ar->capacity = ARRAY_INIT_ALLOC;
		}
		else {
			ar->capacity *= ARRAY_REALLOC_COEF;
		}
		ar->items = realloc(ar->items, ar->size_of_item * ar->capacity);
	}

	memcpy(ar->items + ar->count * ar->size_of_item, item, ar->size_of_item);
	ar->count++;

	return ar;
}

void array_map(void *a, void(*map)(void *)) {
	array *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items + i * ar->size_of_item);
	}
}

void array_map1(void *a, void(*map)(void *, void *), void *arg) {
	array *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items + i * ar->size_of_item, arg);
	}
}


