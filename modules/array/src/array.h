#include <stdlib.h>
#include <stdint.h>

#define ARRAY_REALLOC_COEF 2
#define ARRAY_INIT_ALLOC 16

typedef struct {
	uintptr_t *items;
	size_t count;
	size_t capacity;
	size_t size_of_item;
} array;

array *array_new(size_t capacity, size_t size_of_item);

void array_free(void *a);

array *array_append(void *a, void *i);

void array_map(void *a, void(*map)(void *));

// map with 1 argument
void array_map1(void *a, void(*map)(void *, void *), void *arg);


