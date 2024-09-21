#include <string.h>
#include <stdio.h>
#include "array.h"


array *array_new(size_t capacity, size_t size_of_item) {
	MALLOC(array, res, );
	MALLOCS(res->items, size_of_item * capacity, free(res));
	res->size_of_item = size_of_item;
	res->capacity = capacity;
	return res;
}

// resize items into a new array
array *array_scale(void *a, size_t new_size_of_item) {
	array *ar = a;
	array *res = array_new(ar->capacity, new_size_of_item);
	IF_RNULL(!res, "array_new returned NULL", );

	size_t copy_size;
	ASSIGN_IF(copy_size, new_size_of_item < ar->size_of_item, new_size_of_item, ar->size_of_item);
	
	FOR(i, 0, ar->capacity, 
			memcpy(res->items + i * res->size_of_item, ar->items + i * ar->size_of_item, copy_size););

	return res;
}


