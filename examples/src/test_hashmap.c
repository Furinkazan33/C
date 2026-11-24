#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <search.h>
#include <assert.h>
#include "macros.h"

void add_or_increment(char *key) {
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

int key_freq() {
	#ifdef DEBUG
		puts("key/freq with standard hashmap from search.h");
	#endif
	
	hcreate(10);
	char *keys[] = { "v1", "v2", "v3", "v4" };

	for(size_t i = 0; i < 4; i++) {
		add_or_increment(keys[i]);
	}

	#ifdef DEBUG
		for(size_t i = 0; i < 4; i++) {
			ENTRY *f = hsearch((ENTRY) { .key=keys[i] }, FIND);
			if(f) {
				printf("%s %d\n", f->key, *((int *)f->data));
			}
		}
	#endif
	
	return 0;
}

int key_value() {
	#ifdef DEBUG
		puts("key/value with standard hashmap from search.h");
	#endif
	
	char *keys[] = { "1", "2" };
	char *data[] = { "d1", "d2" };

	// populating hashmap
	hcreate(10);
	for(size_t i = 0; i < 2; i++) {
		hsearch((ENTRY) { .key=keys[i], .data=data[i] }, ENTER);
	}

	#ifdef DEBUG
		// searching
		ENTRY *found;
		if((found = hsearch((ENTRY) { .key=keys[0] }, FIND))) {
			printf("found : %s %s\n", found->key, (char *)found->data);
		}
	#endif
	
	return 0;
}


int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	key_freq();
	
	key_value();
	
}

