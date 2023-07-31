#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btree.h"

#define _POSIX_C_SOURCE 200809L

void foo(__attribute__((unused)) void *data) {}

void print_data(void *data) {
	printf("%s-", (char *)data);
}

int compare(void *d1, void *d2) {
	return strcmp((char *)d1, (char *)d2);
}

int main(void) {
	char *c0 = "root";
	char *c1 = "un";
	char *c2 = "deux";
	btree *t1 = btree_new(c1, NULL, NULL);
	btree *t2 = btree_new(c2, NULL, NULL);
	btree *root = btree_new(c0, t1, t2);

	btree_run_lr_inf(root, print_data);

	puts("");

	printf("Found : %s\n", (char *)btree_find_lr_pre(root, compare, "root"));

	btree_free(root, foo);

	return 0;
}
