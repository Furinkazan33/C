#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "array.h"


static array *_array_alloc(size_t size) {
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

	res->size = size;
	res->n = 0;

	return res;
}

static array *_array_realloc(array *a) {
	assert(a);

	if(a->n >= a->size) {
		fprintf(stdout, "realloc : n=%ld s=%ld new_size=%ld\n", a->n, a->size, a->size * 2);
		void **tmp = realloc(a->data, sizeof(void *) * a->size * 2);

		if(!tmp) {
			perror("array_alloc : realloc data failed");
			return NULL;
		}
		a->size *= 2;
		a->data = tmp;
	}

	return a;
}

array *_array_new(int sorted, sf *sf, size_t init_size) {
	array *res = _array_alloc(init_size);

	if(!res) {
		return NULL;
	}
	res->sorted = sorted;
	res->sf = sf;

	return res;
}

array *array_new_sorted(sf *sf, size_t init_size) {
	return _array_new(1, sf, init_size);
}

array *array_new(sf *sf, size_t init_size) {
	return _array_new(0, sf, init_size);
}

array *array_add(array *a, void *c) {
	assert(a && a->sorted);

	size_t i;
	a = _array_realloc(a);
	if(!a) {
		return NULL;
	}

	for(i = 0; i < a->n; i++) {
		if(a->data[i]) {
			if(a->sf->cmp(a->data[i], c) >= 0) {
				break;
			}
		}
	}

	/* decal */
	for(size_t j = a->n; j > i; j--) {
		a->data[j] = a->data[j-1];
	}
	a->data[i] = c;

	a->n++;

	return a;
}

array *array_add_at(array *a, void *d, size_t idx) {
	assert(!a->sorted);

	a = _array_realloc(a);
	if(!a) {
		return NULL;
	}

	for(size_t i = a->n - 1; i > idx; i--) {
		a->data[i] = a->data[i-1];
	}
	a->data[idx] = d;
	a->n++;

	return a;
}

array *array_append(array *a, void *d) {
	assert(!a->sorted);

	a = _array_realloc(a);
	if(!a) {
		return NULL;
	}

	a->data[a->n++] = d;

	return a;
}

size_t array_idx(array *a, void *search) {
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			if(a->sorted && a->sf->cmp(a->data[i], search) > 0) {
				return -1;
			}
			if(!a->sf->cmp(a->data[i], search)) {
				return i;
			}
		}
	}
	return -1;
}

void *array_find(array *a, void *search) {
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			if(a->sorted && a->sf->cmp(a->data[i], search) > 0) {
				return NULL;
			}
			if(!a->sf->cmp(a->data[i], search)) {
				return a->data[i];
			}
		}
	}
	return NULL;
}

void array_remove(array *a, size_t idx, int with_free) {
	if(with_free) {
		a->sf->free(a->data[idx]);
	}

	a->n--;

	for(size_t i = idx; i < a->n; i++) {
		a->data[i] = a->data[i+1];
	}
}

void array_remove_keep_nulls(array *a, size_t idx, int with_free) {
	if(with_free) {
		a->sf->free(a->data[idx]);
	}
	a->data[idx] = NULL;
}

size_t _next_non_null(array *a, size_t idx) {
	while(idx < a->n && !a->data[idx] ) {
		idx++;
	}
	return idx;
}

array *array_remove_nulls(array *a, int with_resize) {
	size_t i, next;

	for(i = 0, next = 0; i < a->n && next < a->n; i++) {
		if(!a->data[i]) {
			if(next) {
				next = _next_non_null(a, next + 1);
			}
			else {
				next = _next_non_null(a, i + 1);
			}
			if(next < a->n) {
				a->data[i] = a->data[next];
				a->data[next] = NULL;
			}
		}
	}
	a->n -= (next - i + 1);

	if(with_resize && a->n <= a->size / 4) {
		printf("resizing from %ld to %ld\n", a->size, a->size/2);
		void **tmp = realloc(a->data, sizeof(void *) * a->size / 2);
		if(!tmp) {
			perror("");
			return NULL;
		}
		a->data = tmp;
		a->size /= 2;
	}
	return a;
}

void array_free(array *a, int with_data) {
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i] && with_data) {
			a->sf->free(a->data[i]);
		}
	}
	free(a->data);
	free(a->sf);
	free(a);
}

void array_print(array *a) {
	for(size_t i = 0; i < a->n; i++) {
		if(a->data[i]) {
			a->sf->print(a->data[i]);
		}
		else {
			printf("NULL\n");
		}
	}
}


