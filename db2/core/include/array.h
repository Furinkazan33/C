#include <stdlib.h>

/*
 * void * array manipulation.
 *
 * types defined : array
 * prefix used for functions : array_
 *
 */

#define ARRAY_REALLOC_COEF 2

typedef struct array {
	size_t size;					// allocated size
	size_t n;						// number of elements
	void **data;
	
	int (*cmp)(void *, void *);		// compare two elements
	void (*free)(void *);			// free element
	void (*write)(void *, FILE *);	// write element	
} array;


array *array_new(size_t init_size);
array *array_copy(array *a);
array *array_resize(array *a, size_t newsize);
void array_free(array *a, int with_data);			// a->free is called if not NULL

/* 
 * functions over elements
 */
void array_set_cmp(array *a, int (*cmp_data)(void *, void *));
void array_set_free(array *a, void (*free_data)(void *));
void array_set_write(array *a, void (*write_data)(void *, FILE *));

/* 
 * adding element 
 * */
array *array_add_at(array *a, void *d, size_t i);
array *array_append(array *a, void *d);	
array *array_concat(array *res, array *add);
/* free existing element and set new value by ref */
void array_replace(array *a, size_t i, void *value);

/* 
 * moving element
 * */
void array_sort(array *a);							// a->cmp is called
void array_swap(array *a, void *d1, void *d2);
void array_swap_idx(array *a, size_t i1, size_t i2);

/* 
 * retrieving element
 * */
void *array_get(array *a, size_t i);
void *array_find(array *a, void *search);			// a->cmp is called
array *array_find_all(array *a, void *search, size_t init_alloc);

/* 
 * removing element 
 * */
void array_remove_idx(array *l, size_t i, int keep_null);
void array_remove(array *l, void *p, int keep_null);
void array_remove_nulls(array *a);

/* 
 * run through functions 
 * */
void array_write(array *a, FILE *file);				// a->write is called
void array_map(array *a, void (*map)(void *));
/* reduce array to a unique element. result has to be pre-allocated. */
void array_reduce(array *a, void *result, void (*reduce)(void *, void *));


