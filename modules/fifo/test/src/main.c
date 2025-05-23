#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "fifo.h"
#include <message.h>

void free_container(container *c) {
	free(c->data);
	free(c);
}

int main(void) {
	fifo *f = fifo_new();
	message *m;

	printf("creating 5000 messages in fifo\n");
	for(int i = 0; i < 5000; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			return 1;
		}
		if(fifo_push(f, m) == NULL) {
			return 1;
		}
	}

	printf("poping every mesasges from fifo\n");
	m = fifo_pop(f);

	while(m) {
		message_print(m);
		free(m);
		m = fifo_pop(f);
	}

	printf("Asserting that fifo is empty\n");
	assert(f->n == 0 && f->head == NULL && f->tail == NULL);

	printf("creating 50 messages in fifo\n");
	for(int i = 0; i < 50; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			return 1;
		}
		if(fifo_push(f, m) == NULL) {
			return 1;
		}
	}
	
	printf("free\n");
	FIFO_FREE(f, free_container);
	printf("Asserting that fifo is NULL\n");
	assert(f == NULL);

	return 0;
}

