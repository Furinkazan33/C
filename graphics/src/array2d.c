#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array2d.h"

array2d *array2d_new(size_t height, size_t width, size_t size_of_item, void *init) {
	array2d *res;
	MALLOC(res, sizeof(*res), );
	res->height = height;
	res->width = width;
	res->capacity = height * width;
	res->size_of_item = size_of_item;
	MALLOC(res->items, size_of_item * height * width, );
	
	if(init) {
		for(size_t i = 0; i < height * width; i++) {
			memcpy(res->items + i * size_of_item, init, size_of_item);
		}
	}
	return res;
}

array2d *array2d_resize(void *a, size_t width, size_t height) {
	array2d *res = a;
	char *tmp = realloc(res->items, res->size_of_item * width * height);
	IF_PERROR(!tmp, "realloc failed", );
	
	// initializing new lines to 0
	if(height > res->height) {
		FOR(l, res->height, height,
				memset(tmp + l * width * res->size_of_item, 0, res->size_of_item * width););

		// initializing new columns to 0
		if(width > res->width) {
			FFOR(l, 0, res->height, c, res->width, width, 
					memset(tmp + (l * width + c) * res->size_of_item, 0, res->size_of_item););
		}
	}
	else {
		// initializing new columns to 0
		if(width > res->width) {
			FFOR(l, 0, height, c, res->width, width, 
					memset(tmp + (l * width + c) * res->size_of_item, 0, res->size_of_item););
		}
	}

	res->items = tmp;
	res->height = height;
	res->width = width;
	res->capacity = height * width;

	return res;
}

array2d *array2d_scale(void *ar, size_t new_size_of_item) {
	array2d *a = ar;
	array2d *res = array2d_new(a->height, a->width, new_size_of_item, NULL);
	size_t copy_size;
	ASSIGN_IF(copy_size, new_size_of_item < a->size_of_item, new_size_of_item, a->size_of_item);

	FOR(i, 0, a->capacity, memcpy(res->items + i * new_size_of_item, a->items + i * a->size_of_item, copy_size););
	
	return res;
}


array2d *array2d_new_from_file(size_t height, size_t width, size_t size_of_item, FILE *fd) {
	array2d *res = array2d_new(height, width, size_of_item, NULL);
	IF_RNULL(!res, "array2d_new returned NULL", );
	
	int n = fread(res->items, res->capacity * res->size_of_item, 1, fd);
	IF_PERROR(n != 1, "fread failed", );
	
	return res;
}

array2d *array2d_write_to_file(void *ar, FILE *fd) {
	array2d *a = ar;
	int n = fwrite(a->items, a->size_of_item * a->capacity, 1, fd);
	IF_PERROR(n != 1, "fwrite failed", );
	
	return a;
}

