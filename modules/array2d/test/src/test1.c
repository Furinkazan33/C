#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array2d.h"

typedef struct {
	int id;
	int toto;
} person;


person *person_new(int id, int toto) {
	person *res = malloc(sizeof(person));
	res->id = id;
	res->toto = toto;
	return res;
}

person p1 = { .id=1, .toto=10 };
person p2 = { .id=2, .toto=20 };
person p3 = { .id=3, .toto=30 };

void p_print(person *p ) {
	printf("person:%d %d\n", p->id, p->toto);
}

void p_inc_id(person  *p, void *inc) {
	p->id += *(int *)inc;
}

void test_person() {
	puts("test_person");
	ARRAY2D_DECLARE(person, array2d_p);
	array2d_p *a = (array2d_p *) array2d_new(2, 3, sizeof(person), NULL);

	ARRAY2D_SET(a, 0, 0, &p1);
	ARRAY2D_SET(a, 0, 1, &p2);
	ARRAY2D_SET(a, 0, 2, &p3);

	ARRAY2D_MAP(a, p_print);

	int inc=1;
	ARRAY2D_MAP1(a, p_inc_id, &inc);
	ARRAY2D_MAP(a, p_print);

	ARRAY2D_FREE(a);
}

int main(void) {
	test_person();

	return 0;
}

