#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sort_struct_pointers.h>


/*
  Exemple de tri sur un tableau de pointeurs.
  Le tri échange les pointeurs dans le tableau => rapide !
  
gcc sort_struct_pointers.c && time ./a
real    0m2.512s

*/


void person_populate(Person **pointers, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		pointers[i]->id = rand() % MAX_INT;
	}
}

void person_sort(Person **pointers, int size) {
	int i, j;
	int min;
	
	for (i = 0; i < size; i++) {
		for (j = i + 1; j < size; j++) {
			if (pointers[i]->id > pointers[j]->id) {
				Person *p = pointers[j];
				// Inversion
				pointers[j] = pointers[i];
				pointers[i] = p;
			}
		}
	}
}

void persons_pairing(Person **pointers, Person *structures, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		pointers[i] = &structures[i];
	}
}

void person_print(Person **pointers, int size) {
	int i;
	
	for (i = 0; i < size; i++) {
		printf("%d ", pointers[i]->id);
	}
}
