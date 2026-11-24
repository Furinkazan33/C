#include <stdlib.h>
#include <stdbool.h>
#include "macros.h"


#ifndef ARRAY_H
#define ARRAY_H

typedef struct array { 
	size_t item_size;
	size_t capacity;
	size_t count;
	char *items;
} array;

#define ARRAY_REALLOC_COEF 2
#define ARRAY_INIT_ALLOC 16


#define ARRAY_GET(a, i, type) ((type) (a->items + i * a->item_size))

#define ARRAY_DO(a, idx, type, item, block) FOR(idx, 0, a->count, type item = ARRAY_GET(a, idx, type); block)
		
#define ARRAY_FREE(a) free(a->items); free(a)

#endif


array *array_new(size_t capacity, size_t item_size);
void array_free(array *a);

array *array_dump_infos(array *a, FILE *fd);
array *array_dump_items(array *a, FILE *fd, void *(*item_dump)(FILE *, void *));
array *array_dump(array *a, FILE *fd, void *(*item_dump)(FILE *, void *));

array *array_load(FILE *fd, void (*item_load)(void *, FILE *));


array *array_add(array *a, void *item);

array *array_del(array *a, void *data);
array *array_del_idx(array *a, size_t idx);

void *array_find(array *a, void *search, int (*cmp)(const void *, const void *));
int array_find_idx(array *a, void *search, int (*cmp)(const void *, const void *));

void *array_find_rec(array *a, void *search, int (*cmp)(const void *, const void *), size_t *idx);
int array_find_rec_idx(array *a, void *search, int (*cmp)(const void *, const void *), size_t *idx);

array *array_find_all(array *a, void *search, int (*cmp)(const void *, const void *));
array *array_find_all_idx(array *a, void *search, int (*cmp)(const void *, const void *));
