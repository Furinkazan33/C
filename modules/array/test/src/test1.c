#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"

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

void p_print(void *i ) {
	person *p = i;
	printf("person:%d %d\n", p->id, p->toto);
}

void p_inc_id(void *i, void *inc) {
	person *p = i;
	p->id += *(int *)inc;
}

void test_person() {
	puts("test_person");
	array *a = array_new(4, sizeof(person));
	ARRAY_SET(a, 0, &p1);
	ARRAY_SET(a, 1, &p2);
	ARRAY_SET(a, 2, &p3);
	ARRAY_MAP(a, p_print);

	int inc=1;
	ARRAY_MAP1(a, p_inc_id, &inc);
	ARRAY_MAP(a, p_print);

	ARRAY_FREE(a);
}

int main(void) {
	test_person();

	return 0;
}

