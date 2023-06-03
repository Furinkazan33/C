#include <btree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

void foo(__attribute__((unused)) void *data) {}

typedef struct {
	char id[10];
	int age;
	char name[32];
} Person;



void print_data(void *data) {
	printf("%s-", ((Person *) data)->name);
}

int compare(void *data, void *search) {
	return strcmp(((Person *) data)->id,  ((Person *) search)->id);
}

int main(void) {
	Person static_person, *found;
	btree *left, *right, *root;
	
	left = btree_new(&static_person, sizeof(Person), NULL, NULL);
	memcpy(((Person*)(left->data))->id, "1", 32);
	memcpy(((Person*)(left->data))->name, "left name", 32);
	
	right = btree_new(&static_person, sizeof(Person), NULL, NULL);
	memcpy(((Person*)(right->data))->id, "2", 32);
	memcpy(((Person*)(right->data))->name, "right name", 32);
	
	root = btree_new(&static_person, sizeof(Person), left, right);
	memcpy(((Person*)(root->data))->id, "0", 32);
	memcpy(((Person*)(root->data))->name, "root name", 32);

	btree_run_lr_inf(root, print_data);
	
	puts("");
	
	found = btree_find_lr_pre(root, compare, "2");
	
	if(found) {
		printf("Found : %s\n", found->name);
	} else {
		printf("Not found !\n");
	}
	
	btree_delete(root, free);
	
	return 0;
}