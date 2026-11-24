#include <stdio.h>
#include "fifo.h"

void *fifo_new() {
	fifo *f = malloc(sizeof(*f));
	if(!f) {
		perror("fifo_new : alloc failed");
		return NULL;
	}
	f->n = 0;
	f->head = NULL;
	f->tail = NULL;

	return f;
}

void *fifo_push(void *fi, void *m) {
	fifo *f = fi;
	container *c = malloc(sizeof(container));
	if(!c) {
		perror("");
		return NULL;
	}
	c->next = NULL;
	c->data = m;

	/* first push */
	if(!f->tail) {
		f->head = c;
		f->tail = c;
	}
	else {
		f->tail->next = c;
		f->tail = f->tail->next;
	}

	f->n++;

	return f;
}

void *fifo_pop(void *fi) {
	fifo *f = fi;
	void *res;
	container *c;

	/* empty void */
	if(!f->head) {
		return NULL;
	}
	c = f->head;
	res = c->data;
	f->head = f->head->next;
	free(c);
	f->n--;

	/* empty void */
	if(!f->head) {
		f->tail = NULL;
	}

	return res;
}


