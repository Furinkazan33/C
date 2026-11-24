#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "fifo.h"
#include <message.h>


int main(void) {
	fifo *f = fifo_new();
	message *m;

	fprintf(stderr, "=== TEST creation + pop + assert empty ===\n");

	fprintf(stderr, "creating 50 messages in fifo\n");
	for(int i = 0; i < 50; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			return 1;
		}
		if(fifo_push(f, m) == NULL) {
			return 1;
		}
	}

	fprintf(stderr, "poping every mesasges from fifo\n");
	m = fifo_pop(f);

	while(m) {
		message_print(m);
		free(m);
		m = fifo_pop(f);
	}

	fprintf(stderr, "Asserting that fifo is empty\n");
	assert(f->n == 0 && f->head == NULL && f->tail == NULL);


	fprintf(stderr, "=== TEST creation + free + assert NULL ===\n");

	fprintf(stderr, "creating 50 messages in fifo\n");
	for(int i = 0; i < 50; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			return 1;
		}
		if(fifo_push(f, m) == NULL) {
			return 1;
		}
	}
	
	fprintf(stderr, "free\n");
	FIFO_FREE(f, free);
	
	fprintf(stderr, "Asserting that fifo is NULL\n");
	assert(f == NULL);

	return 0;
}

