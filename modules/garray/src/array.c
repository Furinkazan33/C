#include "array.h"

array *array_new(size_t capacity) {
	array *res = malloc(sizeof(*res));
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

void array_free(array *ar, void (*custom_free)(void *)) {
	if(custom_free) {
		for(size_t i = 0; i < ar->count; i++) {
			custom_free(ar->items[i]);
		}
	}
	free(ar->items);
	free(ar);
}

array *array_append(array *ar, void *item) {
	if(ar->count == ar->capacity) {
		ar->capacity *= ARRAY_REALLOC_COEF;
		ar->items = realloc(ar->items, sizeof(*(ar->items)) * ar->capacity);
	}

	ar->items[ar->count++] = item;

	return ar;
}

void array_map(array *ar, void(*map)(void *)) {
	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i]);
	}
}

void array_map1(array *ar, void(*map)(void *, void *), void *arg) {
	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i], arg);
	}
}


