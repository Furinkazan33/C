#include <stdio.h>
#include <string.h>
#include "mem.h"

#define _POSIX_C_SOURCE 200809L

#define NAME_LEN 32
#define PERSONS_SIZE 10

typedef struct {
	uint32_t id;
	char name[32];
} Person;
int print_char(any *s) {
	printf("%s\n", (char *)s);
	return 1;
}
int print_uint32(any *s) {
	printf("%d\n", *(uint32_t *)s);
	return 1;
}
int main(void) {
	Person structures[PERSONS_SIZE];
	char name[NAME_LEN] = "Mathieu";

	for (int i = 0; i < PERSONS_SIZE; i++) {
		structures[i].id = i;
		sprintf(name + 7, "%d", i);
		strcpy(structures[i].name, name);
	}

	mem_map((any *)structures, PERSONS_SIZE, sizeof(Person), print_uint32);
	mem_map((any *)structures[0].name, PERSONS_SIZE, sizeof(Person), print_char);

	return 0;
}
