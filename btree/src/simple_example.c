#include <btree.h>
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


char *data_to_chars(void *data) {
	return (char *) data;
}



int main(void) {
	btree *t1 = btree_new("un\0", 3, NULL, NULL);
	btree *t2 = btree_new("deux\0", 5, NULL, NULL);
	btree *root = btree_new("root\0", 5, t1, t2);
	
	btree_run_lr_inf(root, print_data);
	
	puts("");
		
	printf("Found : %s\n", (char *)btree_find_lr_pre(root, compare, "root"));	
	
	btree_delete(root, free);
	
	return 0;
}