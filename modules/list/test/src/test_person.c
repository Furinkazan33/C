#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "person.h"
#include "list.h"


int main(void) {
	typedef vlist list;

	list *l = NULL;

	person *p1 = person_new(1, 21, "toto 1", "");
	person *p2 = person_new(2, 22, "toto 2", "");
	person *p3 = person_new(3, 23, "toto 3", "");
	person *p4 = person_new(4, 24, "toto 4", "");
	person *p5 = person_new(5, 25, "toto 5", "");
	person *p6 = person_new(6, 26, "toto 6", "");

	l = (list *) list_new(p3); if(!l) { return 1; }
	list *p = l;
	p = list_insert_after(p, p2);
	p = list_insert_after(p, p1);
	p = list_insert_after(p, p4);
	p = list_insert_after(p, p6);
	p = list_insert_after(p, p5);

	LIST_MAP1(l, person_write, stdout);

	LIST_FREE_ALL(l, person_free);

	return 0;
}
