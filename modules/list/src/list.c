#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h"

void *list_new(void *data) {
	vlist *res = malloc(sizeof(vlist));
	if(!res) {
		perror("list_alloc : alloc failed");
		return NULL;
	}
	res->next = NULL;
	res->data = data;

	return res;
}

void *list_insert_after(void *li, void *data) {
	vlist *l = li;

	vlist *next = l->next;
	vlist *new = list_new(data);
	if(!new) {
		fprintf(stderr, "list_insert_after : call to list_new returned NULL\n");
		return NULL;
	}
	l->next = new;
	new->next = next;

	return new;
}

void *list_insert_before(void *li, void *data) {
	vlist *l = li;

	vlist *new = list_new(data);
	if(!new) {
		fprintf(stderr, "list_insert_before : call to list_new returned NULL\n");
		return NULL;
	}
	new->next = l;

	return l;
}

void *list_tail(void *li) {
	vlist *l = li;
	while(l->next) {
		l = l->next;
	}
	return l;
}

void *list_get(void *li, size_t n) {
	vlist *res = li;

	size_t i;
	for(i = 0; i < n && res; i++) {
		res = res->next;
	}

	if(i < n) {
		fprintf(stderr, "list_get : index out of bound : %ld < %ld\n", i, n);
	}

	return res;
}

/* forward void pointer and return element */
void *list_next(void *ref) {
	vlist **res = ref;
	*res = (*res)->next;
	return *res;
}

void *list_copy(void *li, void *(*copy)(void *)) {
	vlist *p = li;
	void *data_cpy = NULL;
	void *first_elt = copy(p->data);
	if(!first_elt) {
		fprintf(stderr, "list_copy : first call to copy returned NULL\n");
		return NULL;
	}
	p = p->next;

	vlist *res = list_new(first_elt);
	if(!res) {
		fprintf(stderr, "list_copy : call to list_new returned NULL\n");
		return NULL;
	}
	vlist *p_res = res;

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
bool list_equal(void *li1, void *li2, bool (*eq)(void *, void *), void **e) {
	vlist *l1 = li1;
	vlist *l2 = li2;

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

