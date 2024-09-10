#include "parray.h"

parray *parray_new(size_t capacity) {
	parray *res = malloc(sizeof(*res));
	res->items = malloc(sizeof(*(res->items)) * capacity);
	res->count = 0;
	res->capacity = capacity;
	return res;
}

void parray_free(void *a, void (*custom_free)(void *)) {
	parray *ar = a;

	if(custom_free) {
		for(size_t i = 0; i < ar->count; i++) {
			custom_free(ar->items[i]);
		}
	}
	free(ar->items);
	free(ar);
}

parray *parray_append(void *a, void *item) {
	parray *ar = a;

	if(ar->count == ar->capacity) {
		if(ar->capacity == 0) {
			ar->capacity = PARRAY_INIT_ALLOC;
		}
		else {
			ar->capacity *= PARRAY_REALLOC_COEF;
		}
		ar->items = realloc(ar->items, sizeof(*(ar->items)) * ar->capacity);
	}

	ar->items[ar->count++] = item;

	return ar;
}

void parray_map(void *a, void(*map)(void *)) {
	parray *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i]);
	}
}

void parray_map1(void *a, void(*map)(void *, void *), void *arg) {
	parray *ar = a;

	for(size_t i = 0; i < ar->count; i++) {
		map(ar->items[i], arg);
	}
}


