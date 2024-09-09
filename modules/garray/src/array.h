#include <stdlib.h>

#define ARRAY_INIT_ALLOC_ITEMS 16
#define ARRAY_REALLOC_COEF 2

#ifndef ARRAY_ITEM
#define ARRAY_ITEM void
#endif

typedef struct {
	ARRAY_ITEM **items;
	size_t count;
	size_t capacity;
} array;

array *array_new();
void array_free(array *ar, void (*free)(ARRAY_ITEM *));
array *array_append(array *ar, ARRAY_ITEM *item);

void array_map(array *ar, void(*map)(ARRAY_ITEM *));
void array_map1(array *ar, void(*map)(ARRAY_ITEM *, void *), void *arg);


