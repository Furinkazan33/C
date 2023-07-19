#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/person.h"
#include "include/btree.h"

#define _POSIX_C_SOURCE 200809L


int main(void) {
	person *found;
	btree *left, *right, *root;

	person *p0 = person_new("0", "p0");
	person *p1 = person_new("1", "p1");
	person *p2 = person_new("2", "p2");

	left = btree_new(p1, NULL, NULL);
	right = btree_new(p2, NULL, NULL);
	root = btree_new(p0, left, right);

	found = btree_find_lr_pre(root, person_cmp, p2);

	if(found) {
		printf("Found : %s\n", found->name);
	} else {
		printf("Not found !\n");
	}

	btree_free(root, person_free);

	return 0;
}
