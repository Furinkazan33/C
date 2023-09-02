#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <message.h>
#include "list.h"


int main(void) {
	list *l;
	message *m;
	container *c;

	l = list_new();
	if(!l) {
		return 1;
	}
	list_set_write(l, message_write);
	list_set_free(l, message_free);
	list_set_cmp(l, message_cmp);

	for(int i = 0; i < 5000; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			break;
		}
		if(!list_append(l, m)) {
			break;
		}
	}
	list_write(l, stdout);
	puts("-----------------");

	c = list_pop(l, l->head);

	while(c) {
		list_free_container(l, c);
		c = list_pop(l, l->head);
	}

	list_write(l, stdout);
	puts("-----------------");

	assert(l->n == 0 && l->head == NULL && l->tail == NULL);

	list_free(l);

	return 0;
}
