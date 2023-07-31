#include <stdlib.h>
#include "sf.h"

#ifndef FIFO_DATA_TYPE
	#define FIFO_DATA_TYPE void
#endif

typedef struct container {
	FIFO_DATA_TYPE *data;
	struct container *next;
} container;


typedef struct fifo {
	size_t n;
	container *head;
	container *tail;
	sf *sf;
} fifo;


fifo *fifo_new(sf *sf);
void fifo_free(fifo *f);

fifo *fifo_push(fifo *f, FIFO_DATA_TYPE *m);
FIFO_DATA_TYPE *fifo_pop(fifo *f);

