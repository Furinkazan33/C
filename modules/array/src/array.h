#include <stdlib.h>
#include <stdint.h>
#include "macros.h"

#ifndef ARRAY_H
#define ARRAY_H

#define ARRAY_DECLARE(type, name) typedef struct { \
	type *items; \
	size_t capacity; \
	size_t size_of_item; \
} name

#define ARRAY_SET(a, i, v) memcpy(((char *)a->items) + i * a->size_of_item, v, a->size_of_item)
#define ARRAY_GET_AS(a, i, type) (type *)(((char *)a->items) + i * a->size_of_item)
#define ARRAY_GET(a, i) ARRAY_GET_AS(a, i, void)
#define ARRAY_FREE(a) free(a->items); free(a)
#define ARRAY_MAP(a, f) FOR(i, 0, a->capacity, f((void *)(((char *)a->items) + i * a->size_of_item));)
#define ARRAY_MAP1(a, f, arg) FOR(i, 0, a->capacity, f((void *)(((char *)a->items) + i * a->size_of_item), arg);)


ARRAY_DECLARE(char, array);

#endif

array *array_new(size_t capacity, size_t size_of_item);
array *array_scale(void *a, size_t new_size_of_item); // resize items into a new array


