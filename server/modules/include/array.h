#include <stdlib.h>
#include "sf.h"

#define ARRAY_REALLOC_COEF 1.5

typedef struct array {
	int sorted;
	size_t size;
	size_t n;
	sf *sf;
	void **data;
} array;


array *array_new_sorted(sf *sf, size_t init_size);
array *array_new(sf *sf, size_t init_size);

/* assert sorted */
array *array_add(array *l, void *c);
/* assert not sorted */
array *array_add_at(array *a, void *d, size_t idx);
array *array_append(array *a, void *d);

size_t array_idx(array *l, void *search);
void *array_find(array *l, void *search);


void array_remove(array *l, size_t idx, int with_free);
void array_remove_keep_nulls(array *l, size_t idx, int with_free);
array *array_remove_nulls(array *a, int with_resize);

void array_free(array *l, int with_data);

void array_print(array *l);

