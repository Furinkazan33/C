#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_INT 100000


typedef struct {
	int id;
	int age;
	char name[32];
} Person;


void person_populate(Person **pointers, int size);
void person_sort(Person **pointers, int size);
void persons_pairing(Person **pointers, Person *structures, int size);
void person_print(Person **pointers, int size);
