#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "macros.h"
#include "list.h"

void list_free(vlist *l, void (*f)(void *)) {
	f(((vlist *) l)->data); 
	free(l);
}

void list_free_all(vlist *l, void (*f)(void *)) {
	LIST_MAP_ARGS(l, list_free, f);
}

vlist *list_new(void *data) {
	vlist *res = malloc(sizeof(vlist));
	if(!res) {
		perror("alloc failed");
		LOG_E("call to malloc returned NULL\n");
		return NULL;
	}
	res->next = NULL;
	res->data = data;

	return res;
}

vlist *list_insert_after(vlist *l, void *data) {
	vlist *next = l->next;
	vlist *new = list_new(data);
	if(!new) {
		LOG_E("call to list_new returned NULL\n");
		return NULL;
	}
	l->next = new;
	new->next = next;

	return new;
}

vlist *list_insert_before(vlist *l, void *data) {
	vlist *new = list_new(data);
	if(!new) {
		LOG_E("call to list_new returned NULL\n");
		return NULL;
	}
	new->next = l;

	return l;
}

void list_remove_next(vlist *l) {
	assert(l->next);
	
	l->next = l->next->next;
}

vlist *list_pop(vlist **l, size_t n) {
	vlist *res = NULL;
	
	if(n == 0) {
		res = *l;
		*l = (*l)->next;
		return res;
	}
	else {
		vlist *p = *l;
		for(size_t i = 0; i < n - 1 && p; i++) {
			p = p->next;
		}
		
		res = p->next;
		p->next = p->next->next;
		
		return res;
	}
}

vlist *list_tail(vlist *l) {
	while(l->next) {
		l = l->next;
	}
	return l;
}

vlist *list_get(vlist *res, size_t n) {
	size_t i;
	for(i = 0; i < n && res; i++) {
		res = res->next;
	}

	if(i < n) {
		LOG_E("index out of bound : %ld < %ld\n", i, n);
		return NULL;
	}

	return res;
}

vlist *list_next(vlist **ref) {
	*ref = (*ref)->next;
	return *ref;
}

vlist *list_copy(vlist *l, void *(*copy)(void *)) {
	void *data_cpy = NULL;
	void *first_elt = copy(l->data);
	if(!first_elt) {
		LOG_E("first call to copy returned NULL\n");
		return NULL;
	}
	l = l->next;

	vlist *res = list_new(first_elt);
	if(!res) {
		LOG_E("call to list_new returned NULL\n");
		return NULL;
	}
	vlist *p_res = res;

	while(l) {
		data_cpy = copy(l->data);
		if(!data_cpy) {
			LOG_E("call to copy returned NULL\n");
			return NULL;
		}

		p_res = list_insert_after(p_res, data_cpy);
		if(!p_res) {
			LOG_E("call to list_insert_after returned NULL\n");
			return NULL;
		}
		l = l->next;
	}

	return res;
}

vlist *find_prec(vlist *l, void *search, int (*eq)(void *, void *)) {
	vlist *prec = NULL;
	
	while(l) {
		if(eq(l->data, search) == 0) {
			return prec;
		}
		else {
			prec = l;
			l = l->next;
		}
	}
	return NULL;
}

vlist *find(vlist *l, void *search, int (*eq)(void *, void *)) {
	while(l) {
		if(eq(l->data, search) == 0) {
			return l;
		}
		else {
			l = l->next;
		}
	}
	return NULL;
}

/* Compare l2 against l1. l1 can be longer than l2
 * e : end elt of l2 */
bool list_equal(vlist *l1, vlist *l2, bool (*eq)(const void *, const void *), vlist **e) {
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

vlist *array_to_list(char *data, size_t count, size_t item_size) {
	vlist *res = list_new(NULL);
	if(!res) {
		LOG_E("vlist_new return NULL\n");
		return NULL;
	}
	
	if(data != NULL && count > 0) {
		res->data = data;
		
		vlist *tmp = res;
		
		FOR(i, 0, count, {
			if(!list_insert_after(tmp, data + item_size * i)) {
				LOG_E("list_insert_after return NULL\n");
				return NULL;
			}
			tmp = tmp->next;
		})
	}
	
	return res;
}

