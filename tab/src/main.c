#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <tab.h>

#define _POSIX_C_SOURCE 200809L

#define NAME_LEN 32
#define PERSONS_SIZE 10

typedef struct {
	uint32_t id;
	char name[NAME_LEN];
} Person;


void print_char(void *s) {
	printf("%s", (char *)s);

	printf("\n");
}

void print_uint4(void *s) {
	printf("%d\n", *(uint32_t *)s);
}


int main(void) {
	Person structures[PERSONS_SIZE];
	int i;
	char name[NAME_LEN] = "Mathieu";
	
	/* Assignation des id et des noms*/
	for (i = 0; i < PERSONS_SIZE; i++) {
		structures[i].id = i;
		sprintf(name + 7, "%d", i);
		memcpy(structures[i].name, name, NAME_LEN);
	}
	
	/* Printing every ids and names */
	tab_sweep((uint8_t *)&structures[0], PERSONS_SIZE, sizeof(Person), print_uint4);
	tab_sweep((uint8_t *)structures[0].name, PERSONS_SIZE, sizeof(Person), print_char);


	return 0;	
}