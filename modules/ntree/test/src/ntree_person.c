#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "person.h"
#define NTREE_DATA_TYPE person
#include "ntree.h"


int main(void) {
	int err = 0;
	person *found;
	person *d0 = person_new(0, 20, "p0", "");
	person *d1 = person_new(1, 21, "p1", "");
	person *d2 = person_new(2, 22, "p2", "");
	person *d3 = person_new(3, 23, "p3", "");
	person *d4 = person_new(4, 24, "p4", "");

	ntree *root = ntree_alloc(NULL); root->data = d0;
	ntree *c1 = ntree_alloc(NULL); c1->data = d1;
	ntree *c2 = ntree_alloc(NULL); c2->data = d2;
	ntree *c3 = ntree_alloc(NULL); c3->data = d3;
	ntree *c4 = ntree_alloc(NULL); c4->data = d4;

	err += ntree_add_child(root, c1);
	err += ntree_add_child(root, c2);
	err += ntree_add_child(root, c3);
	err += ntree_add_child(root, c4);

	if(err > 0) {
		ntree_free(root, person_free);
		return 1;
	}

	ntree_run_lr_pre(root, &person_print);
	puts("");
	found = ntree_find(root, &person_cmp, d2);
	person_print(found);
	puts("");

	char dest[1024] = { 0 };
	ntree_to_s(dest, root, person_to_json);
	printf("%s\n", dest);

	ntree_free(root, person_free);

	return 0;
}
