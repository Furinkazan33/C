#include <ntree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

void foo(__attribute__((unused)) void *data) {}


void print_data(void *data) {
	printf("%s-", (char *) data);
}

int compare(void *d1, void *d2) {
	return strcmp((char *) d1, (char *) d2);
}

int main(void) {
	ntree *root;
	ntree *children[3];
	char *found;
	
	children[0] = ntree_new("un\0", 3, NULL);
	children[1] = ntree_new("deux\0", 5, NULL);
	children[2] = NULL;
	
	root = ntree_new("root\0", 5, children);
	
	
	ntree_run_lr_pre(root, print_data);
	puts("");
	
		
	found = ntree_find(root, compare, "deux");
	
	printf("%s\n", found);
	
	ntree_delete(root, free);
	
	return 0;
}