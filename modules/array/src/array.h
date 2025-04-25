#include <stdlib.h>
#include "macros.h"


#ifndef ARRAY
#define ARRAY

#define ARRAY_REALLOC_COEF 2

typedef struct {
	void *items;
	size_t size;
	size_t count;
	size_t capacity;
} array;

#endif

array *array_new(size_t size, size_t capacity);
array *array_add(array *a, void *item);
void *array_get(array *a, size_t i);
void array_free(array *a);


