/*
 *
 * Simple implementation of a 2d array with resizeable height and width
 *
 */

#include <stdlib.h>
#include <stdint.h>
#include <helpers.h>

#ifndef ARRAY2D

#define ARRAY2D

#define ARRAY2D_DECLARE(type, name) typedef struct { \
	type *items; \
	size_t height; \
	size_t width; \
	size_t capacity; \
	size_t size_of_item; \
} name

ARRAY2D_DECLARE(char, array2d);

#define ARRAY2D_FREE(a) free(a->items); free(a)
#define ARRAY2D_SET(a, l, c, v) memcpy(((char *)a->items) + (l * a->width + c) * a->size_of_item, v, a->size_of_item)
#define ARRAY2D_GET(a, l, c) (void *)(((char *)a->items) + l * a->width + c)
#define ARRAY2D_GET_AS(a, l, c, type) (type *)(((char *)a->items) + l * a->width + c)

#define ARRAY2D_MAP(a, f) FOR(i, 0, a->capacity, f((void *)(((char *)a->items) + i * a->size_of_item));)
#define ARRAY2D_MAP1(a, f, arg) FOR(i, 0, a->capacity, f((void *)(((char *)a->items) + i * a->size_of_item), arg);)
#define ARRAY2D_MAP2(a, f, arg1, arg2) FOR(i, 0, a->capacity, f((void *)(((char *)a->items) + i * a->size_of_item), arg1, arg2);)

#define ARRAY2D_MAP_L(a, l, f) FOR(c, 0, a->width, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)));)
#define ARRAY2D_MAP_L1(a, l, f, arg) FOR(c, 0, a->width, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)), arg);)
#define ARRAY2D_MAP_L2(a, l, f, arg1, arg2) FOR(c, 0, a->width, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)), arg1, arg2);)

#define ARRAY2D_MAP_C(a, c, f) FOR(l, 0, a->height, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)));)
#define ARRAY2D_MAP_C1(a, c, f, arg) FOR(l, 0, a->height, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)), arg);)
#define ARRAY2D_MAP_C2(a, c, f, arg1, arg2) FOR(l, 0, a->height, f((void *)(((char *)a->items) + a->size_of_item * (l * a->width + c)), arg1, arg2);)

#endif

array2d *array2d_new(size_t height, size_t width, size_t size_of_item, void *init);
array2d *array2d_new_from_file(size_t height, size_t width, size_t size_of_item, FILE *fd);
array2d *array2d_resize(void *a, size_t new_width, size_t new_height);
array2d *array2d_scale(void *a, size_t new_size_of_item); // resize items into a new array
array2d *array2d_write_to_file(void *a, FILE *fd);


