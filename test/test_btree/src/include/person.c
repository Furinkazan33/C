#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "person.h"

#define p_cast(p) ((person *)p)

void person_print(void *p) {
	printf("%s %s\n", p_cast(p)->id, p_cast(p)->name);
}

int person_cmp(void *p1, void *p2) {
	return strcmp(p_cast(p1)->id, p_cast(p2)->id);
}

person *person_new(char *id, char *name) {
	person *res = (person *)malloc(sizeof(person));
	if(!res) {
		perror("person_new : alloc failed");
		return NULL;
	}

	strncpy(res->id, id, PERSON_STR_LEN);
	strncpy(res->name, name, PERSON_STR_LEN);

	return res;
}

void person_free(void *p) {
	free(p_cast(p));
}


