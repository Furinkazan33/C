#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

#define NTREE_DATA_TYPE char
#include "ntree.h"

void foo(__attribute__((unused)) char *data) {}


void print_data(char *data) {
	printf("%s-", data);
}

int compare(char *d1, char *d2) {
	return strcmp(d1, d2);
}

int main(void) {
	int err = 0;
	char *found;
	char *d0 = "root";
	char *d1 = "un";
	char *d2 = "deux";
	//char *d3 = "trois";
	char *d4 = "quatre";
	char *d5 = "cinq";

	ntree *root = ntree_alloc(NULL); root->data = d0;
	ntree *c1 = ntree_alloc(NULL); c1->data = d1;
	ntree *c2 = ntree_alloc(NULL); c2->data = d2;
	ntree *c3 = ntree_alloc(NULL);
	ntree *c4 = ntree_alloc(NULL); c4->data = d4;
	ntree *c5 = ntree_alloc(NULL); c5->data = d5;

	err += ntree_add_child(root, c1);
	err += ntree_add_child(root, c2);
	err += ntree_add_child(root, c3);
	err += ntree_add_child(root, c4);
	err += ntree_add_child(root, c5);

	if(err > 0) {
		ntree_free(root, foo);
		return 1;
	}

	ntree_run_lr_pre(root, &print_data);
	puts("");

	found = ntree_find(root, &compare, "deux");

	printf("%s\n", found);

	ntree_free(root, foo);

	return 0;
}
