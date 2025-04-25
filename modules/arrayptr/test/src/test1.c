#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arrayptr.h"

typedef struct {
	int id;
	int toto;
} person;

void p_print(person *p) {
	printf("person:%d %d\n", p->id, p->toto);
}

void p_inc_id(person *p, void *inc) {
	p->id += *(int *)inc;
}

int main(void) {
	typedef arrayptr parray;

	parray *a = (parray *) arrayptr_new(10);

	person p1 = { .id=1, .toto=10 };
	person p2 = { .id=2, .toto=20 };
	person p3 = { .id=3, .toto=30 };
	arrayptr_append(a, &p1);
	arrayptr_append(a, &p2);
	arrayptr_append(a, &p3);

	ARRAYPTR_MAP(a, p_print);

	int inc=1;
	ARRAYPTR_MAP1(a, p_inc_id, &inc);
	ARRAYPTR_MAP(a, p_print);

	ARRAYPTR_FREE(a);

	return 0;
}

