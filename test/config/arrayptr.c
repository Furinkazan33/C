#include <stdio.h>
#include "arrayptr.h"

arrayptr *arrayptr_new(size_t capacity) {
	MALLOC(arrayptr, res, );
	MALLOCS(res->items, sizeof(*(res->items)) * capacity, free(res););
	res->count = 0;
	res->capacity = capacity;
	return res;
}

arrayptr *arrayptr_append(void *a, void *item) {
	arrayptr *ar = a;

	if(ar->count == ar->capacity) {
		if(ar->capacity == 0) {
			ar->capacity = ARRAYPTR_INIT_ALLOC;
		}
		else {
			ar->capacity *= ARRAYPTR_REALLOC_COEF;
		}
		ar->items = realloc(ar->items, sizeof(*(ar->items)) * ar->capacity);
	}

	ar->items[ar->count++] = item;

	return ar;
}

