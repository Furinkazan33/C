#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sort_struct_pointers.h>

#define _POSIX_C_SOURCE 200809L

#define TAB_SIZE 30000

/*
  Exemple de tri sur un tableau de pointeurs.
  Le tri échange les pointeurs dans le tableau => rapide !
  
gcc sort_struct_pointers.c && time ./a
real    0m2.512s

*/


int main(void) {
	Person structures[TAB_SIZE]; // une seule allocation mémoire
	Person *pointers[TAB_SIZE];
	
	persons_pairing(pointers, &structures[0], TAB_SIZE);
	person_populate(pointers, TAB_SIZE);
	person_sort(pointers, TAB_SIZE);
	//person_print(pointers, TAB_SIZE);
	
	return 0;
}