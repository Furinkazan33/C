#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include "macros.h"

void insert(char *key) {
	ENTRY *found;
	if((found = hsearch((ENTRY) { .key=key }, FIND))) {
		(*((int *)found->data))++;
	}
	else {
		int *f = malloc(sizeof(int));
		*f = 1;
		hsearch((ENTRY) { .key=key, .data=f }, ENTER);
	}
}

int test_1() {
	puts("key/freq with standard hashmap from search.h");

	hcreate(10);
	char *keys[] = { "v1", "v2", "v3", "v4" };

	insert(keys[0]);
	insert(keys[1]);
	insert(keys[1]);
	insert(keys[2]);

	for(size_t i = 0; i < 4; i++) {
		ENTRY *f = hsearch((ENTRY) { .key=keys[i] }, FIND);
		if(f) {
			printf("%s %d\n", f->key, *((int *)f->data));
		}
	}

	return 0;
}

int test_2() {
	puts("key/value with standard hashmap from search.h");

	char *keys[] = { "1", "2" };
	char *data[] = { "d1", "d2" };

	// populating hashmap
	hcreate(10);
	for(size_t i = 0; i < 2; i++) {
		hsearch((ENTRY) { .key=keys[i], .data=data[i] }, ENTER);
	}

	// searching
	ENTRY *found;
	if((found = hsearch((ENTRY) { .key=keys[0] }, FIND))) {
		printf("found : %s %s\n", found->key, (char *)found->data);
	}

	return 0;
}


int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	TEST_INIT(argv[0]);
	TEST(1);
	TEST(2);
	TEST_RESULTS();
}

