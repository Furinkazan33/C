#include <stdlib.h>
#include <helpers.h>


#ifndef ARRAYPTR_H
#define ARRAYPTR_H

#define ARRAYPTR_DECLARE(type, name) typedef struct { \
	type **items; \
	size_t count; \
	size_t capacity; \
} name

#define ARRAYPTR_REALLOC_COEF 2
#define ARRAYPTR_INIT_ALLOC 16

#define ARRAYPTR_GET(a, i) a->items[i]
#define ARRAYPTR_GET_AS(a, i, type) (type *)(a->items[i])

#define ARRAYPTR_FREE(a) free(a->items); free(a)
#define ARRAYPTR_FREE_F(a, f) FOR(i, 0, a->count, f(a->items[i]);); free(a->items); free(a)

#define ARRAYPTR_MAP(a, f) FOR(i, 0, a->count, f(a->items[i]);)
#define ARRAYPTR_MAP1(a, f, arg) FOR(i, 0, a->count, f(a->items[i], arg);)

ARRAYPTR_DECLARE(void, arrayptr);

#endif


arrayptr *arrayptr_new(size_t capacity);
arrayptr *arrayptr_append(void *a, void *i);


