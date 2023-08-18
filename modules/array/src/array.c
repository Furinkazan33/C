#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "array.h"


static array *_array_alloc(size_t size) {
	assert(size > 0);

	array *res = malloc(sizeof(array));
	if(!res) {
		perror("array_alloc : alloc failed");
		return NULL;
	}

	res->data = malloc(sizeof(void *) * size);
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

array *array_new(size_t init_size) {
	array *res = _array_alloc(init_size);

	if(!res) {
		return NULL;
	}
	res->size = init_size;
	res->n = 0;
	res->cmp = NULL;
	res->free = NULL;
	res->print = NULL;
	res->tos = NULL;

	return res;
}

void array_set_cmp(array *a, int (*cmp_data)(void *, void *)) {
	a->cmp = cmp_data;
}
void array_set_free(array *a, void (*free_data)(void *)) {
	a->free = free_data;
}
void array_set_print(array *a, void (*print_data)(void *)) {
	a->print = print_data;
}
void array_set_tos(array *a, void (*tos_data)(void *)) {
	a->tos = tos_data;
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

	a->data[a->n] = d;
	a->n++;

	return a;
}

array *array_concat(array *res, array *add) {
	assert(res);
	assert(add);

	for(size_t i = 0; i < add->n; i++) {
		if(!array_append(res, add->data[i])) {
			return NULL;
		}
	}
	return res;
}

array *array_add_keep_sorted(array *a, void *d) {
	assert(a);
	assert(a->cmp);
	size_t i;

	for(i = 0; i < a->n; i++) {
		if(a->cmp(a->data[i], d) >= 0) {
			break;
		}
	}

	return array_add_at(a, d, i);
}


void array_sort(array *res) {
	size_t min = 0;
	for(size_t i = 0; i < res->n; i++) {
		// looking for min
		for(size_t j = i + 1; j < res->n; j++) {
			if(res->cmp(res->data[min], res->data[j]) > 0) {
				min = j;
			}
		}
		array_swap_idx(res, min, i);
	}
}

void array_swap(array *a, void *d1, void *d2) {
	assert(a);
	assert(d1);
	assert(d2);

	void *tmp = d1;
	d1 = d2;
	d2 = tmp;
}

void array_swap_idx(array *a, size_t idx1, size_t idx2) {
	assert(a);
	assert(idx1 < a->n);
	assert(idx2 < a->n);

	void *tmp = a->data[idx1];
	a->data[idx1] = a->data[idx2];
	a->data[idx2] = tmp;
}

void *array_find(array *a, void *search) {
	assert(a);
	assert(a->cmp);

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
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
	assert(a->print);

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			a->print(a->data[i]);
		}
	}
}

void array_free(array *a, int with_data) {
	assert(a);
	assert(a->free);

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

	array *res = array_new(a->size);
	if(!res) {
		perror("array_copy : call to _array_new returned NULL");
		return NULL;
	}
	res->cmp = a->cmp;
	res->free = a->free;
	res->print = a->print;

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


