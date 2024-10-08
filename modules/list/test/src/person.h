#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PERSON_NAME_LEN 32
#define PERSON_ADRESS_LEN 128

#define p_cast(p) ((person *)p)

typedef struct person {
	int id;
	int age;
	char name[PERSON_NAME_LEN];
	char adress[PERSON_ADRESS_LEN];
} person;

int person_write(person *p, void *file);
int person_cmp(person *p1, person *p2);
void person_free(person *p);
person *person_copy(person *p);

person *person_new(int id, int age, char *name, char *adress);

void person_to_json(char *dest, person *p);


