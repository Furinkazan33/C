#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "fifo.h"
#include <message.h>

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

	free(f);

	return 0;
}

