#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PERSON_STR_LEN 32

typedef struct person {
	char id[PERSON_STR_LEN];
	char name[PERSON_STR_LEN];
} person;

void person_print(void *p);
int person_cmp(void *p1, void *p2);
person *person_new(char *id, char *name);
void person_free(void *p);

