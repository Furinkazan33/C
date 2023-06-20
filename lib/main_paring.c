#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define _POSIX_C_SOURCE 200809L

#define TAB_SIZE 50

typedef struct person {
	int id;
	int age;
	char name[32];
} person;

/*
  Le tri échange les pointeurs dans le tableau => rapide !
*/

/* Pairing pointers to structures */
void person_pairing(person *p[TAB_SIZE + 1], person *s) {
	int i;

	for(i=0; i<TAB_SIZE; i++) {
		p[i] = s++;
	}
}

void person_populate(person *p[TAB_SIZE + 1], int n) {
	int i;

	assert(n < TAB_SIZE);

	for(i=0; i<n; i++) {
		p[i]->id = i;
		p[i]->age = 0;
		strcpy(p[i]->name, "TOTO");
	}
	p[i] = NULL;
}

void person_print(person **p) {
	while(p && *p) {
		printf("%d %d %s\n", (*p)->id, (*p)->age, (*p)->name);
		p++;
	}
}

int main(void) {
	person structures[TAB_SIZE]; // une seule allocation mémoire
	person *pointers[TAB_SIZE + 1];
	person *last = &structures[0];

	person_pairing(pointers, &structures[0]);
	person_populate(pointers, 10);
	person_print(pointers);

	return 0;
}
