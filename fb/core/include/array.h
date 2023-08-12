#include <stdlib.h>
#include "sf.h"

/*
 * array manipulation.
 *
 * array is an array of void *
 *
 * types defined : array
 * prefix used for functions : array.
 *
 */


#define ARRAY_REALLOC_COEF 1.5

typedef struct array {
	size_t size;
	size_t n;
	sf *sf;
	void **data;
} array;


array *array_new(sf *sf, size_t init_size);
array *array_sort(array *a); //TODO: implement
array *array_add_at(array *a, void *d, size_t idx);
array *array_append(array *a, void *d);
array *array_add_keep_sorted(array *l, void *d);
array *array_swap(array *a, void *d1, void *d2);
array *array_swap_idx(array *a, size_t idx1, size_t idx2);

array *array_copy(array *a);
size_t array_idx(array *l, void *search);
void *array_find(array *l, void *search);
void array_remove_idx(array *l, size_t idx, int keep_null, int with_free);
void array_remove(array *l, void *p, int keep_null, int with_free);
/* Resize with size/2 if a->n < a->size/4 */
array *array_remove_nulls(array *a);
array *array_resize(array *a, size_t newsize);
void array_free(array *l, int with_data);
void array_print(array *l);

/* call map on every array elements, array is changed */
void array_map(array *a, void (*map)(void *));
/* reduce array to a unique element, array is unchanged */
void array_reduce(array *a, void *result, void (*reduce)(void *, void *));

