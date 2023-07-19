#include <stdio.h>
#include "person.h"
#include "list.h"

int main(void) {
	P_SF(p_sf)
	list *l = list_new((list_sf *) &p_sf);

	if(!l) {
		return 1;
	}

	person p1, p2, p3; p1.id = 1; p2.id = 2; p3.id=3;

	list_add(l, &p3); list_add(l, &p2); list_add(l, &p1);
	list_add(l, &p3); list_add(l, &p2); list_add(l, &p1);

	list_print(l);
	puts("-----------------");

	list_remove_keep_nulls(l, 2);
	list_remove_keep_nulls(l, 4);

	list_print(l);
	puts("-----------------");

	person *found = list_find(l, &p3);
	if(found) {
		printf("found : %d\n", found->id);
	}

	list_free(l);

	return 0;
}
