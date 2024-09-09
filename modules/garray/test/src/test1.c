#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int id;
	int toto;
} person;
#define ARRAY_ITEM person
#include "array.h"


person p1 = { .id=1, .toto=10 };
person p2 = { .id=2, .toto=20 };
person p3 = { .id=3, .toto=30 };

void p_print(person *p ) {
	printf("person:%d %d\n", p->id, p->toto);
}

void p_inc_id(person *p, void *inc) {
	p->id += *(int *)inc;
}

int main(void) {
	array *ar = array_new(0);

	array_append(ar, &p1);
	array_append(ar, &p2);
	array_append(ar, &p3);
	array_map(ar, p_print);

	int inc=1;
	array_map1(ar, p_inc_id, &inc);
	array_map(ar, p_print);

	array_free(ar, NULL);

	return 0;
}

