#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

list *list_new(void *data) {
	list *res = malloc(sizeof(list));
	if(!res) {
		perror("list_alloc : alloc failed");
		return NULL;
	}
	res->next = NULL;
	res->data = data;

	return res;
}

list *list_insert_after(list *l, void *data) {
	assert(l);

	list *next = l->next;
	list *new = list_new(data);
	if(!new) {
		fprintf(stderr, "list_insert_after : call to list_new returned NULL\n");
		return NULL;
	}
	l->next = new;
	new->next = next;

	return new;
}

list *list_insert_before(list *l, void *data) {
	assert(l);

	list *new = list_new(data);
	if(!new) {
		fprintf(stderr, "list_insert_before : call to list_new returned NULL\n");
		return NULL;
	}
	new->next = l;

	return l;
}

list *list_tail(list *l) {
	assert(l);

	list *p;
	while(p->next) {
		p = p->next;
	}

	return p;
}

list *list_get(list *l, size_t n) {
	assert(l);

	list *res = l;

	size_t i;
	for(i = 0; i < n && res; i++) {
		res = res->next;
	}

	if(i < n) {
		fprintf(stderr, "list_get : index out of bound : %ld < %ld\n", i, n);
	}

	return res;
}

/* forward list pointer and return element */
list *list_next(list **ref) {
	*ref = (*ref)->next;
	return *ref;
}

void list_free(list *l, void (*data_free)(void *)) {
	assert(l);
	assert(l->data);
	assert(data_free);

	if(data_free) {
		data_free(l->data);
	}
	free(l);
}

void list_free_all(list *l, void (*data_free)(void *)) {
	assert(l);
	assert(data_free);

	list *next = NULL;
	for(list *p = l; p; p = next) {
		next = p->next;
		list_free(p, data_free);
	}
}

list *list_copy(list *l, void *(*copy)(void *)) {
	assert(l);
	assert(l->data);

	list *p = l;
	void *data_cpy = NULL;
	void *first_elt = copy(p->data);
	if(!first_elt) {
		fprintf(stderr, "list_copy : first call to copy returned NULL\n");
		return NULL;
	}
	p = p->next;

	list *res = list_new(first_elt);
	if(!res) {
		fprintf(stderr, "list_copy : call to list_new returned NULL\n");
		return NULL;
	}
	list *p_res = res;

	while(p) {
		data_cpy = copy(p->data);
		if(!data_cpy) {
			fprintf(stderr, "list_copy : call to copy returned NULL\n");
			return NULL;
		}

		p_res = list_insert_after(p_res, data_cpy);
		if(!p_res) {
			fprintf(stderr, "list_copy : call to list_insert_after returned NULL\n");
			return NULL;
		}
		p = p->next;
	}

	return res;
}

/* Compare l2 against l1. l1 can be longer than l2
 * e : end elt of l2 */
bool list_equal(list *l1, list *l2, bool (*eq)(void *, void *), list **e) {
	assert(l1);
	assert(l2);

	while(l2) {
		if(!l1 || !eq(l1->data, l2->data)) {
			*e = l1;
			return false;
		}
		l1 = l1->next;
		l2 = l2->next;
	}
	*e = l1;
	return true;
}

void list_map_void(list *l, void (*map)(void *)) {
	assert(l);
	assert(map);

	list *p = l;

	while(p) {
		map(p->data);
		p = p->next;
	}
}

void list_map_void2(list *l, void (*map)(void *, void *), void *param) {
	assert(l);
	assert(map);

	list *p = l;

	while(p) {
		map(p->data, param);
		p = p->next;
	}
}

int list_map(list *l, int (*map)(void *)) {
	assert(l);
	assert(map);

	list *p = l;
	int rc;

	while(p) {
		rc = map(p->data);
		if(!rc) {
			return rc;
		}
		p = p->next;
	}
	return 1;
}

int list_map2(list *l, int (*map)(void *, void *), void *param) {
	assert(l);
	assert(map);

	list *p = l;
	int rc;

	while(p) {
		rc = map(p->data, param);
		if(!rc) {
			return rc;
		}
		p = p->next;
	}
	return 1;
}

