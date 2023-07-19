#include <stdio.h>
#include "person.h"


void person_free(void *p) {
	(void) p;
}

int person_cmp(void *p1, void *p2) {
	return p_cast(p1)->id - p_cast(p2)->id;
}

void person_print(void *p) {
	printf("%d\n", p_cast(p)->id);
}


