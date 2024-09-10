#include <stdlib.h>

#define PARRAY_REALLOC_COEF 2
#define PARRAY_INIT_ALLOC 16

typedef struct {
	void **items;
	size_t count;
	size_t capacity;
} parray;

parray *parray_new(size_t capacity);
void parray_free(void *a, void (*free)(void *));
parray *parray_append(void *a, void *i);

void parray_map(void *a, void(*map)(void *));
void parray_map1(void *a, void(*map)(void *, void *), void *arg);


