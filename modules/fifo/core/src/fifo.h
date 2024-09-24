#include <stdlib.h>

#ifndef FIFO_h
#define FIFO_h

#define FIFO_DECLARE(data_type, fifo_name, container_name) typedef struct container_name {\
	data_type *data;\
	struct container_name *next;\
} container_name;\
typedef struct fifo_name {\
	size_t n;\
	container_name *head;\
	container_name *tail;\
} fifo_name

#define FIFO_FREE(fi, func) { \
	fifo *f = fi; \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		func(c->data);\
		free(c);\
		c = f->head;\
	}\
	free(f); \
}


FIFO_DECLARE(void, fifo, container);

#endif


void *fifo_new();
void *fifo_push(void *f, void *m);
void *fifo_pop(void *f);

