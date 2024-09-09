#include <stdlib.h>

#define ARRAY_REALLOC_COEF 2

typedef struct {
	void **items;
	size_t count;
	size_t capacity;
} sarray;

sarray *sarray_new(size_t capacity);
void sarray_free(void *a, void (*free)(void *));
sarray *sarray_append(void *a, void *i);

void sarray_map(void *a, void(*map)(void *));
void sarray_map1(void *a, void(*map)(void *, void *), void *arg);


