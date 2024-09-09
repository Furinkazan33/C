#include "sarray.h"

sarray *sarray_new(size_t capacity) {
	sarray *res = malloc(sizeof(*res));
	res->count = 0;
	if(capacity > 0) {
		res->items = malloc(sizeof(*(res->items)) * capacity);
	}
	else {
		res->items=NULL;
	}
	res->capacity = capacity;
	return res;
}

void sarray_free(void *a, void (*custom_free)(void *)) {
	sarray *ar = a;

	if(custom_free) {
		for(size_t i = 0; i < ar->count; i++) {
			custom_free(ar->items[i]);
		}
	}
	free(ar->items);
	free(ar);
}

sarray *sarray_append(void *a, void *item) {
	sarray *ar = a;

	if(ar->count == ar->capacity) {
		ar->capacity *= ARRAY_REALLOC_COEF;
		ar->items = realloc(ar->items, sizeof(*(ar->items)) * ar->capacity);
	}

	ar->items[ar->count++] = item;

	return ar;
}

void sarray_map(void *a, void(*map)(void *)) {
	sarray *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i]);
	}
}

void sarray_map1(void *a, void(*map)(void *, void *), void *arg) {
	sarray *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i], arg);
	}
}


