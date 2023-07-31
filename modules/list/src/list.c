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
	res->head = NULL;
	res->tail = NULL;
	res->sf = NULL;
	res->n = 0;

	return res;
}

container *list_container_new(container *prev, container *next, void *data) {
	container *res = malloc(sizeof(container));
	if(!res) {
		perror("list_container_new : alooc failed");
		return NULL;
	}
	res->data = data;
	res->next = next;
	res->prev = prev;
	return res;
}

list *list_new(sf *sf) {
	assert(sf);
	list *res = _list_alloc();

	if(!res) {
		return NULL;
	}
	res->sf = sf;

	return res;
}

container *list_insert_after(list *l, container *after, void *data) {
	assert(l);
	container *c;

	if(after == NULL) {
		c = list_container_new(NULL, NULL, data);
		if(!c) {
			return NULL;
		}

		l->head = c;
		l->tail = c;
	}
	else {
		c = list_container_new(after, after->next, data);
		if(!c) {
			return NULL;
		}
		after->next = c;

		if(l->tail == after) {
			l->tail = c;
		}
	}

	l->n++;

	return c;
}

container *list_append(list *l, void *data) {
	assert(l);
	return list_insert_after(l, l->tail, data);
}

container *list_insert_before(list *l, container *before, void *data) {
	assert(l);
	container *c;

	if(before == NULL) {
		c = list_container_new(NULL, NULL, data);
		if(!c) {
			return NULL;
		}

		l->head = c;
		l->tail = c;
	}
	else {
		c = list_container_new(before->prev, before, data);
		if(!c) {
			return NULL;
		}
		before->prev = c;

		if(l->head == before) {
			l->head = c;
		}
	}

	l->n++;

	return c;
}

container *list_prepend(list *l, void *data) {
	assert(l);
	return list_insert_before(l, l->head, data);
}

container *list_remove(list *l, container *c) {
	assert(l);

	if(l->head == NULL) {
		return NULL;
	}
	if(c->prev) {
		c->prev->next = c->next;
	}
	if(c->next) {
		c->next->prev = c->prev;
	}
	if(l->head == c) {
		l->head = l->head->next;
	}
	if(l->tail == c) {
		l->tail = l->tail->prev;
	}

	l->n--;

	return c;
}

container *list_find(list *l, void *search) {
	assert(l && search);
	container *c = l->head;

	while(c) {
		if(l->sf->cmp(c->data, search) == 0) {
			return c;
		}
		c = c->next;
	}
	return NULL;
}

void list_swap(list *l, container *c1, container *c2) {
	assert(l && c1 && c2);

	if(l->tail == c1) {
		l->tail = c2;
	}
	if(l->tail == c2) {
		l->tail = c1;
	}
	if(l->head == c1) {
		l->head = c2;
	}
	if(l->head == c2) {
		l->head = c1;
	}

	container *prev1 = c1->prev;
	container *next1 = c1->next;

	c1->prev = c2->prev;
	c1->next = c2->next;

	c2->prev = prev1;
	c2->next = next1;
}

void list_delete(list *l, container *c) {
	assert(l && c);
	list_remove(l, c);
	list_free_container(l, c);
}

void list_free_container(list *l, container *c) {
	assert(c);
	l->sf->free(c->data);
}

void list_free(list *l) {
	assert(l);
	container *c;

	while(l->head) {
		c = l->head;
		list_free_container(l, c);
		l->head = l->head->next;
		free(c);
	}
	free(l->sf);
	free(l);
}

void list_print(list *l) {
	assert(l);
	container *c = l->head;

	while(c) {
		l->sf->print(c->data);
		c = c->next;
	}
}


