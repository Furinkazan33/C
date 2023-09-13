#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "array.h"


static array *array_alloc(size_t size) {
	assert(size > 0);

	array *res = malloc(sizeof(array));
	if(!res) {
		perror("array_alloc : call to malloc for array returned NULL");
		return NULL;
	}

	res->data = malloc(sizeof(void *) * size);
	if(!res->data) {
		perror("array_alloc : call to malloc for data returned NULL");
		return NULL;
	}

	return res;
}

static array *array_realloc(array *a) {
	assert(a);

	if(a->n >= a->size) {
#ifdef DEBUG
		fprintf(stdout, "array_realloc : %ld >= %ld\n", a->n, a->size);
#endif
		void **tmp = realloc(a->data, sizeof(void *) * a->size * ARRAY_REALLOC_COEF);

		if(!tmp) {
			perror("array_alloc : call to realloc for data returned NULL");
			return NULL;
		}
		a->size *= ARRAY_REALLOC_COEF;
		a->data = tmp;
#ifdef DEBUG
		fprintf(stdout, "array_realloc : newsize = %ld\n", a->size);
#endif
	}

	return a;
}

array *array_new(size_t init_size) {
	array *res = array_alloc(init_size);

	if(!res) {
		fprintf(stderr, "array_new : alloc failed\n");
		return NULL;
	}
	res->size = init_size;
	res->n = 0;
	res->cmp = NULL;
	res->free = NULL;
	res->write = NULL;

	return res;
}

array *array_add_at(array *a, void *d, size_t idx) {
	assert(a);

	if(!array_realloc(a)) {
		fprintf(stderr, "array_add_at : call to array_realloc returned NULL\n");
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

	if(!array_realloc(a)) {
		fprintf(stderr, "array_append : call to array_realloc returned NULL\n");
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
			fprintf(stderr, "array_add_at : call to array_append returned NULL\n");
			return NULL;
		}
	}

	return res;
}

/* free existing element and set new value by ref */
void array_replace(array *a, size_t i, void *value) {
	assert(a);
	assert(i < a->n);

	if(a->data[i]) {
		if(a->free) {
			a->free(a->data[i]);
		}
		else {
			free(a->data[i]);
		}
	}
	a->data[i] = value;
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
			int compare = a->cmp(a->data[i], search);

			if(compare == 0) {
				return a->data[i];
			}
		}
	}
	return NULL;
}

array *array_find_all(array *a, void *search, size_t init_alloc) {
	assert(a);
	assert(a->cmp);

	array *res = array_new(init_alloc);
	if(!res) {
		fprintf(stderr, "array_find_all : call to array_new returned NULL\n");
		return NULL;
	}

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			int compare = a->cmp(a->data[i], search);

			if(compare == 0) {
				if(!array_append(res, a->data[i])) {
					fprintf(stderr, "array_find_all : call to array_append returned NULL\n");
					return NULL;
				}
			}
		}
	}
	return res;
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

#ifdef DEBUG
	fprintf(stdout, "array_resize : %ld to %ld\n", a->size, newsize);
#endif

	void **tmp = realloc(a->data, sizeof(void *) * newsize);
	if(!tmp) {
		perror("array_resize : call to realloc for data returned NULL");
		return NULL;
	}
	a->data = tmp;
	a->size = newsize;

	return a;
}

void array_remove_nulls(array *a) {
	assert(a);

	for(size_t i = 0; i < a->n; ) {
		if(!a->data[i]) {
			array_remove_idx(a, i, 0);
		}
		else {
			i++;
		}
	}
}

void array_write(array *a, FILE *file) {
	assert(a);
	assert(a->write);

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			a->write(a->data[i], file);
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

array *array_copy(array *a, void *(*cpy)(void *)) {
	assert(a);
	array *res = NULL;
	void *data = NULL;

	res = array_new(a->size);
	if(!res) {
		fprintf(stderr, "array_copy : call to array_new(%ld) returned NULL\n", a->size);
		return NULL;
	}
	res->cmp = a->cmp;
	res->free = a->free;
	res->write = a->write;

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			data = cpy(a->data[i]);
			if(!data) {
				fprintf(stderr, "array_copy : call to copy function returned NULL\n");
				return NULL;
			}
		}
		else {
			data = NULL;
		}

		if(!array_append(res, data)) {
			fprintf(stderr, "array_copy : call to array_append returned NULL\n");
			return NULL;
		}
	}

	return res;
}

void array_map(array *a, void *(*map)(void *)) {
	assert(a);
	assert(map);

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			a->data[i] = map(a->data[i]);
		}
	}
}

void array_reduce(array *a, void *res, void (*reduce)(void *, void *)) {
	assert(a);

	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			reduce(res, a->data[i]);
		}
	}
}

