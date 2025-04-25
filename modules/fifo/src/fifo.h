#include <stdlib.h>


typedef struct container {
	void *data;
	struct container *next;
} container;

typedef struct fifo {
	size_t n;
	container *head;
	container *tail;
} fifo;

#define FIFO_MAP_C(f, func) { \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		func(c);\
		c = f->head;\
	}\
}

#define FIFO_MAP(f, func) { \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		func(c->data);\
		c = f->head;\
	}\
}

#define FIFO_FREE(f, free_container) { \
	FIFO_MAP_C(f, free_container); \
	free(f); \
	f = NULL; }



void *fifo_new();
void *fifo_push(void *f, void *m);
void *fifo_pop(void *f);

