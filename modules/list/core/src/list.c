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
	res->prev = NULL;
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
	new->prev = l;
	new->next = next;

	if(next) {
		next->prev = new;
	}
	
	return new;
}

list *list_insert_before(list *l, void *data) {
	assert(l);

	list *prev = l->prev;
	list *new = list_new(data);
	if(!new) {
		fprintf(stderr, "list_insert_before : call to list_new returned NULL\n");
		return NULL;
	}
	l->prev = new;
	new->next = l;
	new->prev = prev;

	if(prev) {
		prev->next = new;
	}
	
	return l;
}

list *list_head(list *l) {
	assert(l);

	list *p;
	while(p->prev) {
		p = p->prev;
	}

	return p;
}

list *list_tail(list *l) {
	assert(l);

	list *p;
	while(p->next) {
		p = p->next;
	}

	return p;
}

void list_pop(list *l) {
	assert(l);

	list *prev = l->prev;
	list *next = l->next;

	if(prev) {
		prev->next = next;
	}
	if(next) {
		next->prev = prev;
	}
}

void list_free(list *l, void (*data_free)(void *)) {
	assert(l);
	assert(l->data);
	assert(data_free);

	data_free(l->data);
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

void list_map(list *l, void (*map)(void *)) {
	list *p = l;

	while(p) {
		map(p->data);
		p = p->next;
	}
}

void list_map2(list *l, void (*map)(void *, void *), void *param) {
	list *p = l;

	while(p) {
		map(p->data, param);
		p = p->next;
	}
}

