#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include "arrayptr.h"

typedef struct {
	int id;
	int toto;
} person;

void p_print(person *p) {
	printf("person:%d %d\n", p->id, p->toto);
}

int cmp(const void *pe1, const void *pe2) {
	const person *p1 = pe1;
	const person *p2 = *((const person **) pe2);
	return p1->toto - p2->toto;
}

int main(void) {
	puts("creating array of persons");
	arrayptr *a = arrayptr_new(10);
	{
		person p1 = { .id=1, .toto=10 };
		person p2 = { .id=2, .toto=20 };
		person p3 = { .id=3, .toto=10 };
		arrayptr_append(a, &p1);
		arrayptr_append(a, &p2);
		arrayptr_append(a, &p3);

		ARRAYPTR_MAP(a, p_print);
	}

	person search = { .id=0, .toto=10 };
	puts("\nlfind toto=10");
	{
		person **f = lfind(&search, a->items, &a->count, sizeof(person *), cmp);
		if(f && *f) {
			p_print(*f);
		}
	}

	puts("\nlfind loop toto=10");
	{
		void **p = a->items;
		size_t count = a->count;
		while((p = lfind(&search, p, &count, sizeof(person *), cmp)) && count > 0) {
			p_print(*p);
			count = a->count - (++p - a->items);
		}
	}

	puts("\narrayptr_find_all toto=10");
	{
		arrayptr *found = arrayptr_find_all(a, &search, cmp);
		ARRAYPTR_MAP(found, p_print);
		ARRAYPTR_FREE(found);
	}

	ARRAYPTR_FREE(a);

	return 0;
}

