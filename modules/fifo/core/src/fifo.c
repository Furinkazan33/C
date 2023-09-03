#include <stdio.h>
#include "fifo.h"

fifo *fifo_new() {
	fifo *f = (fifo *)malloc(sizeof(*f));
	if(!f) {
		perror("fifo_new : alloc failed");
		return NULL;
	}
	f->n = 0;
	f->head = NULL;
	f->tail = NULL;

	return f;
}

fifo *fifo_push(fifo *f, FIFO_DATA_TYPE *m) {
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

FIFO_DATA_TYPE *fifo_pop(fifo *f) {
	FIFO_DATA_TYPE *res;
	container *c;

	/* empty fifo */
	if(!f->head) {
		return NULL;
	}
	c = f->head;
	res = c->data;
	f->head = f->head->next;
	free(c);
	f->n--;

	/* empty fifo */
	if(!f->head) {
		f->tail = NULL;
	}

	return res;
}

void fifo_free(fifo *f, void (*free_data)(FIFO_DATA_TYPE *)) {
	FIFO_DATA_TYPE *m;
	container *c = f->head;

	while(c) {
		m = c->data;
		f->head = f->head->next;
		free_data(m);
		free(c);
		c = f->head;
	}
	free(f);
}

