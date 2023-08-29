#include <stdlib.h>

/*
 * array manipulation.
 *
 * array is an array of void *
 *
 * types defined : array
 * prefix used for functions : array.
 *
 */


//TODO: improve searching by adding a boolean whether the array is sorted or not :
// after calling sort => bool is true
// when adding an element that unsorted the array => false
// When searching for an element, if cmp returns > 0, then return NULL

#define ARRAY_REALLOC_COEF 2

typedef struct array {
	size_t size;
	size_t n;
	int (*cmp)(void *, void *);
	void (*free)(void *);
	void (*print)(void *);
	void (*tos)(void *);
	void **data;
} array;

 /* allocations */
array *array_new(size_t init_size);
array *array_copy(array *a);
array *array_resize(array *a, size_t newsize);
void array_free(array *a, int with_data);

/* functions iterated through all elements when needed */
void array_set_cmp(array *a, int (*cmp_data)(void *, void *));
void array_set_free(array *a, void (*free_data)(void *));
void array_set_print(array *a, void (*print_data)(void *));
void array_set_tos(array *a, void (*tos_data)(void *));

/* adding */
array *array_add_keep_sorted(array *a, void *d); // a->cmp is called
array *array_add_at(array *a, void *d, size_t idx);
array *array_append(array *a, void *d);
array *array_concat(array *res, array *add);

/* moving elements */
void array_sort(array *a); // a->cmp is called
void array_swap(array *a, void *d1, void *d2);
void array_swap_idx(array *a, size_t idx1, size_t idx2);

/* retrieving */
void *array_get(array *a, size_t i);
void *array_find(array *a, void *search);

/* Modifying elements */
void array_set(array *a, size_t i, void *value);

/* removing */
void array_remove_idx(array *l, size_t idx, int keep_null);
void array_remove(array *l, void *p, int keep_null);
array *array_remove_nulls(array *a);

/* run through functions */
void array_print(array *a);
void array_map(array *a, void (*map)(void *));/* call map on every array elements */
void array_reduce(array *a, void *result, void (*reduce)(void *, void *));/* reduce to a unique element */


