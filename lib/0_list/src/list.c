#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"


static list *_list_alloc() {
	list *res = malloc(sizeof(list));

	if(!res) {
		perror("list_alloc : alloc failed");
		return NULL;
	}

	res->data = malloc(sizeof(void *) * LIST_INIT_SIZE);

	if(!res->data) {
		perror("list_alloc : alloc data failed");
		free(res);
		return NULL;
	}

	res->max_n = LIST_INIT_SIZE;
	res->n = 0;

	return res;
}

static list *_list_realloc(list *l) {
	assert(l);

	if(l->n >= l->max_n) {
		void **tmp = realloc(l->data, sizeof(void *) * l->max_n * LIST_REALLOC_COEF);

		if(!tmp) {
			perror("list_alloc : realloc data failed");
			return NULL;
		}
		l->max_n *= LIST_REALLOC_COEF;
		l->data = tmp;
	}

	return l;
}

list *_list_new(int sorted, list_sf *sf) {
	list *res = _list_alloc();

	if(!res) {
		return NULL;
	}
	res->sorted = sorted;
	res->f = sf;

	return res;
}

list *list_new_sorted(list_sf *sf) {
	return _list_new(1, sf);
}

list *list_new(list_sf *sf) {
	return _list_new(0, sf);
}

int list_add(list *l, void *c) {
	assert(l);

	size_t i;
	l = _list_realloc(l);
	if(!l) {
		return 1;
	}

	if(l->sorted) {
		for(i = 0; i < l->n && l->f->cmp(l->data[i], c) < 0; i++) {
		}

		/* decal */
		for(size_t j = l->n; j > i; j--) {
			l->data[j] = l->data[j-1];
		}
		l->data[i] = c;
	}
	else {
		l->data[l->n] = c;
	}

	l->n++;

	return 0;
}

int list_idx(list *l, void *search) {
	for(size_t i = 0; i < l->n; i++) {
		if(l->data[i]) {
			if(l->sorted && l->f->cmp(l->data[i], search) > 0) {
				return -1;
			}
			if(!l->f->cmp(l->data[i], search)) {
				return i;
			}
		}
	}
	return -1;
}

void *list_find(list *l, void *search) {
	for(size_t i = 0; i < l->n; i++) {
		if(l->data[i]) {
			if(l->sorted && l->f->cmp(l->data[i], search) > 0) {
				return NULL;
			}
			if(!l->f->cmp(l->data[i], search)) {
				return l->data[i];
			}
		}
	}
	return NULL;
}

void list_remove(list *l, size_t idx) {
	size_t i;
	l->n--;
	for(i = idx; i < l->n; i++) {
		l->f->free(l->data[i]);
		l->data[i] = l->data[i+1];
	}
	l->f->free(l->data[i]);
	l->data[i] = NULL;
}

void list_remove_keep_nulls(list *l, size_t idx) {
	l->f->free(l->data[idx]);
	l->data[idx] = NULL;
}

void list_free(list *l) {
	for(size_t i = 0; i < l->n; i++) {
		if(l->data[i]) {
			l->f->free(l->data[i]);
		}
	}
	free(l->data);
	free(l);
}

void list_print(list *l) {
	for(size_t i = 0; i < l->n; i++) {
		if(l->data[i]) {
			l->f->print(l->data[i]);
		}
		else {
			printf("NULL\n");
		}
	}
}


