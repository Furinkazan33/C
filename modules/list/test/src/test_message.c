#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <message.h>
#include <list.h>

FUNC_SF(message)

int main(void) {
	list *l = list_new(message_sf());
	message *m;
	container *c;

	if(!l) {
		return 1;
	}

	for(int i = 0; i < 5000; i++) {
   		m = message_new(i, "Hello you !");
		if(!m) {
			break;
		}
		if(!list_append(l, m)) {
			break;
		}
	}
	list_print(l);
	puts("-----------------");

	c = list_remove(l, l->head);

	while(c) {
		list_free_container(l, c);
		c = list_remove(l, l->head);
	}

	list_print(l);
	puts("-----------------");

	assert(l->n == 0 && l->head == NULL && l->tail == NULL);

	list_free(l);

	return 0;
}
