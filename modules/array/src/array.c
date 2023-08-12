#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "array.h"


static array *_array_alloc(size_t size) {
	array *res = malloc(sizeof(array));

	if(!res) {
		perror("array_alloc : alloc failed");
		return NULL;
	}

	if(size == 0) {
		res->data = NULL;
	}
	else {
		res->data = malloc(sizeof(void *) * size);
	}

	if(!res->data) {
		perror("array_alloc : alloc data failed");
		free(res);
		return NULL;
	}

	return res;
}

static array *_array_realloc(array *a) {
	assert(a);

	if(a->n >= a->size) {
		void **tmp = realloc(a->data, sizeof(void *) * a->size * ARRAY_REALLOC_COEF);

		if(!tmp) {
			perror("array_alloc : realloc data failed");
			return NULL;
		}
		a->size *= ARRAY_REALLOC_COEF;
		a->data = tmp;
	}

	return a;
}

array *array_new(size_t init_size,\
				int (*cmp)(void *, void *),\
				void (*free_data)(void *),\
				void (*print_data)(void *)) {
	array *res = _array_alloc(init_size);

	if(!res) {
		return NULL;
	}
	res->size = init_size;
	res->n = 0;
	res->cmp = cmp;
	res->free = free_data;
	res->print = print_data;

	return res;
}

array *array_add_at(array *a, void *d, size_t idx) {
	assert(a);

	if(!_array_realloc(a)) {
		return NULL;
	}

	for(size_t i = a->n; i > idx; i--) {
		a->data[i] = a->data[i-1];
	}

	a->data[idx] = d;
	a->n++;

	return a;
}

array *array_append(array *a, void *d) {
	assert(a);

	if(!_array_realloc(a)) {
		return NULL;
	}

	a->data[a->n++] = d;

	return a;
}

array *array_add_keep_sorted(array *a, void *d) {
	size_t i;

	for(i = 0; i < a->n; i++) {
		if(a->cmp(a->data[i], d) >= 0) {
			break;
		}
	}

	return array_add_at(a, d, i);
}

array *array_swap(array *a, void *d1, void *d2) {
	assert(a);
	void *tmp = d1;
	d1 = d2;
	d2 = tmp;

	return a;
}

array *array_swap_idx(array *a, size_t idx1, size_t idx2) {
	assert(a);
	return array_swap(a, a->data[idx1], a->data[idx2]);
}


size_t array_idx(array *a, void *search) {
	assert(a);
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			if(a->cmp(a->data[i], search) > 0) {
				return -1;
			}
			if(!a->cmp(a->data[i], search)) {
				return i;
			}
		}
	}
	return -1;
}

void *array_find(array *a, void *search) {
	assert(a);
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			if(a->cmp(a->data[i], search) > 0) {
				return NULL;
			}
			if(!a->cmp(a->data[i], search)) {
				return a->data[i];
			}
		}
	}
	return NULL;
}

void array_remove_idx(array *a, size_t idx, int keep_null) {
	assert(a);
	assert(idx < a->n);

	if(keep_null) {
		a->data[idx] = NULL;
	}
	else {
		a->n--;

		for(size_t i = idx; i < a->n; i++) {
			a->data[i] = a->data[i+1];
		}
	}
}

void array_remove(array *a, void *p, int keep_null) {
	assert(a);
	size_t idx = (char *)p - (char *)(a->data[0]);
	array_remove_idx(a, idx, keep_null);
}

size_t _next_non_null(array *a, size_t idx) {
	assert(a);
	while(idx < a->n && !a->data[idx] ) {
		idx++;
	}
	return idx;
}

array *array_resize(array *a, size_t newsize) {
	assert(a);
	void **tmp = realloc(a->data, sizeof(void *) * newsize);
	if(!tmp) {
		perror("");
		return NULL;
	}
	a->data = tmp;
	a->size = newsize;

	return a;
}

array *array_remove_nulls(array *a) {
	assert(a);
	for(size_t i = 0; i < a->n; ) {
		if(!a->data[i]) {
			array_remove_idx(a, i, 0);
		}
		else {
			i++;
		}
	}

	return a;
}

void array_print(array *a) {
	assert(a);
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			a->print(a->data[i]);
		}
	}
}

void array_free(array *a, int with_data) {
	assert(a);
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i] && with_data) {
			a->free(a->data[i]);
		}
	}
	free(a->data);
	free(a);
}

array *array_copy(array *a) {
	assert(a);
	array *res = array_new(a->size, a->cmp, a->free, a->print);
	if(!res) {
		perror("array_copy : call to _array_new returned NULL");
		return NULL;
	}
	memcpy(res->data, a->data, sizeof(void **) * a->size);
	res->n = a->n;

	return res;
}

void array_map(array *a, void (*map)(void *)) {
	assert(a);
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			map(a->data[i]);
		}
	}
}

void array_reduce(array *a, void *res, void (*reduce)(void *, void *)) {
	assert(a);

	for(size_t i = 0; i < a->n; i++) {
		reduce(res, a->data[i]);
	}
}


