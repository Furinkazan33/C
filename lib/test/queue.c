#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define STR_MAX_LEN 1024
#define FIFO_MAX_MESS 5000


enum mess_type { NONE = 0, INFO, WARN, ERROR };

typedef struct message {
	int priority;
	enum mess_type type;
	char content[STR_MAX_LEN];
	struct message *next;
} message;

typedef struct fifo {
	int n;
	message *push;
	message *pop;
} fifo;

fifo *fifo_new() {
	fifo *f = (fifo *)malloc(sizeof(*f));
	f->n = 0;
	f->pop = NULL;
	f->push = NULL;

	return f;
}

message *tab[150];

fifo *fifo_push(fifo *f, message *m) {
	assert(f && m);

	/* first message */
	if(!f->pop && !f->push) {
		f->pop = m;
		f->push = f->pop;
	}
	else {
		f->push->next = m;
		f->push = f->push->next;
	}

	f->n++;

	return f;
}

message *fifo_pop(fifo *f) {
	assert(f);
	message *res;

	/* empty fifo */
	if(!f->pop && !f->push) {
		return NULL;
	}

	res = f->pop;
	f->pop = res->next;
	f->n--;

	/* empty fifo */
	if(!f->pop) {
		f->push = NULL;
	}

	return res;
}

void fifo_free(fifo *f) {
	assert(f);
	message *m;

	while(f->pop) {
		m = f->pop;
		f->pop = f->pop->next;
		free(m);
	}
}

message *mess_new(int p, enum mess_type t, char *s) {
	message *res = (message *)malloc(sizeof(message));

	res->priority = p;
	res->type = t;
	strcpy(res->content, s);

	return res;
}

int main(void) {
	fifo *f = fifo_new();
	message *m;
	char buffer[256];

	for(int i = 0; i < FIFO_MAX_MESS; i++) {
		sprintf(buffer, "Hello you ! %d", i);
		fifo_push(f, mess_new(i, INFO, buffer));
	}

	m = fifo_pop(f);

	while(m) {
		printf("%d %d %s\n", m->type, m->priority, m->content);
		free(m);
		m = fifo_pop(f);
	}

	assert(f->n == 0 && !f->pop && !f->push);

	fifo_free(f);

	return 0;
}
