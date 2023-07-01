#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define FIFO_ALLOC_SIZE 10
#define FIFO_REALLOC_COEF 2
#define MESS_MAX_LEN 128

enum mess_type { NONE = 0, INFO, WARN, ERROR };

typedef struct message {
	int priority;
	enum mess_type type;
	char content[MESS_MAX_LEN];
} message;

typedef struct fifo {
	int n;
	int size;
	message *messages;
} fifo;

fifo *fifo_create() {
	fifo *f = (fifo *)malloc(sizeof(fifo));

	if(!f) {
		return NULL;
	}

	f->n = 0;
	f->size = FIFO_ALLOC_SIZE;
	f->messages = (message *)malloc(sizeof(message) * f->size);

	return f;
}

message *fifo_pop(fifo **f) {
	assert(*f);
	assert((*f)->n > 0);

	(*f)->n--;
	return ((*f)->messages)+((*f)->n);
}

void *fifo_push(fifo **f, message *m) {
	assert(*f);

	message *p = (*f)->messages;

	/* Realloc */
	if((*f)->n >= (*f)->size) {
		(*f)->size *= FIFO_REALLOC_COEF;

		printf("%d\n", (*f)->size);

		p = (message *)realloc(p, sizeof(message) * (*f)->size);
		if(!p) {
			return NULL;
		}
	}

	memcpy(p + (*f)->n, m, sizeof(message));
	(*f)->n++;

	return p;
}

int main(void) {
	fifo *f = fifo_create();

	message *m = (message *)malloc(sizeof(message));
	strcpy(m->content, "Hello you ! 1");
	m->type = INFO;
	m->priority = 10;

	fifo_push(&f, m);

	strcpy(m->content, "Hello you ! 2");
	fifo_push(&f, m);

	fifo_push(&f, &((message) { NONE, 15, "Hello you ! 3" }));

	while(f->n > 0) {
		m = fifo_pop(&f);
		printf("%d %d %s\n", m->type, m->priority, m->content);
	}

	return 0;
}
