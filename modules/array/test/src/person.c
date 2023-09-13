#include <assert.h>
#include "person.h"

void person_write(void *p, FILE *file) {
	person *pers = p;
	fprintf(file, "%d %d %s %s\n", pers->id, pers->age, pers->name, pers->adress);
}

int person_cmp(void *p1, void *p2) {
	person *pers1 = p1;
	person * pers2 = p2;
	return pers1->id - pers2->id;
}

void person_free(void *p) {
	free((person *)p);
}

person *person_new(int id, int age, char *name, char *adress);

void *person_copy(void *p) {
	person *pp = p;
	return person_new(pp->id, pp->age, pp->name, pp->adress);
}

person *person_new(int id, int age, char *name, char *adress) {
	person *res = (person *)malloc(sizeof(person));
	if(!res) {
		perror("person_new : alloc failed");
		return NULL;
	}

	res->id = id;
	res->age = age;
	strncpy(res->name, name, PERSON_NAME_LEN);
	strncpy(res->adress, adress, PERSON_ADRESS_LEN);

	return res;
}

void person_to_json(char *dest, person *p) {
	char buffer[128];
	dest[0] = '\0';
	strcat(dest, "{ \"id\": ");
	sprintf(buffer, "%d", p->id);
	strcat(dest, buffer);

	strcat(dest, ", \"age\": ");
	sprintf(buffer, "%d", p->age);
	strcat(dest, buffer);

	strcat(dest, ", \"name\": \"");
	strcat(dest, p->name);

	strcat(dest, "\", \"adress\": \"");
	strcat(dest, p->adress);
	strcat(dest, "\" }\0");
}



