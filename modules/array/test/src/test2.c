#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"

typedef struct {
	int id;
	int toto;
} person;


void p_print(person *p) {
	printf("person:%d %d\n", p->id, p->toto);
}

int main(void) {
	array *a = array_new(sizeof(person), 10);

	person p1 = { .id=1, .toto=10 };
	person p2 = { .id=2, .toto=20 };
	person p3 = { .id=3, .toto=10 };

	array_add(a, &p1);
	array_add(a, &p2);
	array_add(a, &p3);

	for(size_t i = 0; i < a->count; i++) {
		p_print(array_get(a, i));
	}

	array_free(a);


	return 0;
}

