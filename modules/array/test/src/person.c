#include <assert.h>
#include "person.h"

void person_write(person *p, FILE *file) {
	fprintf(file, "%d %d %s %s\n", p->id, p->age, p->name, p->adress);
}

int person_cmp(person *p1, person *p2) {
	return p1->id - p2->id;
}

void person_free(person *p) {
	free(p);
}

person *person_new(int id, int age, char *name, char *adress);
person *person_copy(person *p) {
	return person_new(p->id, p->age, p->name, p->adress);
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



