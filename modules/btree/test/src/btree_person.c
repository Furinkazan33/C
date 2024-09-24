#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"

#define BTREE_DATA_TYPE person
#include "btree.h"

#define _POSIX_C_SOURCE 200809L


int main(void) {
	person *found;

	person *p0 = person_new(0, 10, "p0", "");
	person *p1 = person_new(1, 11, "p1", "");
	person *p2 = person_new(2, 12, "p2", "");

	btree *left = btree_new(p1, NULL, NULL);
	btree *right = btree_new(p2, NULL, NULL);
	btree *root = btree_new(p0, left, right);

	found = BTREE_FIND(root, person_cmp, p2);

	if(found) {
		printf("Found : %s\n", found->name);
	} else {
		printf("Not found !\n");
	}

	BTREE_FREE(root, person_free);

	return 0;
}
