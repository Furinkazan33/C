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

#define VA_ARGS(...) , ##__VA_ARGS__

#define FIFO_DO(f, c, block) { \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		block; \
		c = f->head;\
	}\
}

#define FIFO_MAP(f, func, ...) { \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		func(c VA_ARGS(__VA_ARGS__));\
		c = f->head;\
	}\
}

#define FIFO_MAP_DATA(f, func, ...) { \
	container *c = f->head; \
	while(c) { \
		f->head = f->head->next;\
		func(c->data VA_ARGS(__VA_ARGS__));\
		c = f->head;\
	}\
}

#define FIFO_FREE(f, free_data) { \
	FIFO_DO(f, c, { free_data(c->data); free(c); }); \
	free(f); \
	f = NULL; \
}



void *fifo_new();
void *fifo_push(void *f, void *m);
void *fifo_pop(void *f);

