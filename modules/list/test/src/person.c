#include <assert.h>
#include "person.h"

int person_write(void *p, void *file) {
	fprintf((FILE *)file, "%d %d %s %s\n", 
			((person *)p)->id, 
			((person *)p)->age, 
			((person *)p)->name, 
			((person *)p)->adress);

	return 1;
}

int person_cmp(void *p1, void *p2) {
	return ((person *)p1)->id - ((person *)p2)->id;
}

void person_free(void *p) {
	free((person *)p);
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



