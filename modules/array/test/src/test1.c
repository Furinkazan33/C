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
	array *ar = array_new(0, sizeof(person));

	array_append(ar, &p1);
	array_append(ar, &p2);
	array_append(ar, &p3);
	array_map(ar, p_print);

	int inc=1;
	array_map1(ar, p_inc_id, &inc);
	array_map(ar, p_print);

	array_free(ar);
}

int main(void) {
	test_person();

	return 0;
}

