#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parray.h"

typedef struct {
	int id;
	int toto;
} person;

person p1 = { .id=1, .toto=10 };
person p2 = { .id=2, .toto=20 };
person p3 = { .id=3, .toto=30 };

void p_print(void *i ) {
	person *p = i;
	printf("person:%d %d\n", p->id, p->toto);
}

void p_inc_id(void *i, void *inc) {
	person *p = i;
	p->id += *(int *)inc;
}

int main(void) {
	parray *ar = parray_new(0);

	parray_append(ar, &p1);
	parray_append(ar, &p2);
	parray_append(ar, &p3);
	
	parray_map(ar, p_print);

	int inc=1;
	parray_map1(ar, p_inc_id, &inc);
	parray_map(ar, p_print);

	parray_free(ar, NULL);

	return 0;
}

