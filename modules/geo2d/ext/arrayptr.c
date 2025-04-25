#include <stdio.h>
#include <search.h>
#include "arrayptr.h"

arrayptr *arrayptr_new(size_t capacity)	{
	arrayptr *res;
	MALLOC(res, sizeof(*res), );
	MALLOC(res->items, sizeof(*(res->items)) * capacity, free(res); return NULL);
	res->count = 0;
	res->capacity = capacity;
	return res;
}

arrayptr *arrayptr_append(void *array, void *item) {
	arrayptr *a = array;

	if(a->count == a->capacity) {
		REALLOC(a->items, sizeof(*(a->items)) * a->capacity * ARRAYPTR_REALLOC_COEF, return NULL);
		a->capacity *= ARRAYPTR_REALLOC_COEF;
	}

	a->items[a->count++] = item;

	return a;
}

void *arrayptr_find(void *array, void *search, int (*cmp)(void *, void *)) {
	arrayptr *a = array;
	FOR(i, 0, a->count, if(!cmp(a->items[i], search)) return a->items[i];);

	return NULL;
}

void *arrayptr_find_rec(void *array, void *search, int (*cmp)(void *, void *), size_t *idx) {
	arrayptr *a = array;

	while(*idx < a->count) {
		if(!cmp(a->items[(*idx)++], search)) return a->items[*idx - 1];
	}

	return NULL;
}

/*
// using a loop through
arrayptr *arrayptr_find_all(void *array, void *search, int (*cmp)(void *, void *)) {
	arrayptr *res = arrayptr_new(ARRAYPTR_INIT_ALLOC);
	if(!res) {
		LOGE("arrayptr_new returned NULL");
		return NULL;
	}

	{
		void *found = NULL;
		for(size_t i = 0; (found = arrayptr_find_rec(array, search, cmp, &i)); ) {
			if(!arrayptr_append(res, found)) {
				LOGE("failed to append found");
				ARRAYPTR_FREE(res);
				return NULL;
			}
		}
	}

	return res;
}
*/

// using std lfind function
arrayptr *arrayptr_find_all(void *array, void *search, int (*cmp)(const void *, const void *)) {
	arrayptr *res = arrayptr_new(ARRAYPTR_INIT_ALLOC);
	if(!res) {
		LOGE("arrayptr_new returned NULL");
		return NULL;
	}

	{
		arrayptr *a = array;
		void **p = a->items;
		size_t count = a->count;
		while((p = lfind(search, p, &count, sizeof(void *), cmp)) && count > 0) {
			if(!arrayptr_append(res, *p)) {
				LOGE("failed to append found");
				ARRAYPTR_FREE(res);
				return NULL;
			}
			count = a->count - (++p - a->items);
		}
	}

	return res;
}

