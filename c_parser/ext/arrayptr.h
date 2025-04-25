#include <stdlib.h>
#include "macros.h"


#ifndef ARRAYPTR_H
#define ARRAYPTR_H

typedef struct { 
	void **items;
	size_t count;
	size_t capacity;
} arrayptr;

#define ARRAYPTR_REALLOC_COEF 2
#define ARRAYPTR_INIT_ALLOC 16

#define ARRAYPTR_GET(a, i) a->items[i]
#define ARRAYPTR_GET_AS(a, i, type) (type *)(a->items[i])

#define ARRAYPTR_FREE(a) free(a->items); free(a)
#define ARRAYPTR_FREE_F(a, f) FOR(i, 0, a->count, f((void *)(a->items[i]));); free(a->items); free(a)

#define ARRAYPTR_MAP(a, f) FOR(i, 0, a->count, f((void *)(a->items[i]));)
#define ARRAYPTR_MAP1(a, f, arg) FOR(i, 0, a->count, f((void *)(a->items[i]), arg);)


#endif


arrayptr *arrayptr_new(size_t capacity);
arrayptr *arrayptr_append(void *array, void *item);
void *arrayptr_find(void *array, void *search, int (*cmp)(void *, void *));
void *arrayptr_find_rec(void *array, void *search, int (*cmp)(void *, void *), size_t *idx);
arrayptr *arrayptr_find_all(void *array, void *search, int (*cmp)(void *, void *));


