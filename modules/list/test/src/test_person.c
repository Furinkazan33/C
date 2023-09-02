#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "person.h"
#include "list.h"


int main(void) {
	list *l = list_new();
	if(!l) {
		return 1;
	}
	list_set_free(l, person_free);
	list_set_cmp(l, person_cmp);
	list_set_write(l, person_write);

	person *p1 = person_new(1, 21, "toto 1", "");
	person *p2 = person_new(2, 22, "toto 2", "");
	person *p3 = person_new(3, 23, "toto 3", "");
	person *p4 = person_new(4, 24, "toto 4", "");
	person *p5 = person_new(5, 25, "toto 5", "");
	person *p6 = person_new(6, 26, "toto 6", "");

	list_append(l, p3); list_append(l, p2); list_append(l, p1);
	list_append(l, p4); list_append(l, p6); list_append(l, p5);
	list_write(l, stdout);
	puts("-----------------");

	container *c = list_find(l, p4);
	list_delete(l, c);
	list_write(l, stdout);
	puts("-----------------");

	c = list_find(l, p2);
	list_delete(l, c);
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
