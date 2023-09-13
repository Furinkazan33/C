#include <stdio.h>
#include <fcntl.h>
#include "array.h"
#include "person.h"

#define INIT_SIZE 10

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *a = array_new(INIT_SIZE);
	a->cmp = person_cmp;
	a->free = person_free;
	a->write = person_write;


	person *p[10];
	p[8] = person_new(0, 10, "Momo", "route du truc");
	p[7] = person_new(1, 20, "Momo", "route du truc");
	p[5] = person_new(2, 30, "Momo", "route du truc");
	p[6] = person_new(4, 50, "Momo", "route du truc");
	p[9] = person_new(5, 60, "Momo", "route du truc");
	p[3] = person_new(6, 70, "Momo", "route du truc");
	p[4] = person_new(7, 80, "Momo", "route du truc");
	p[2] = person_new(8, 90, "Momo", "route du truc");
	p[0] = person_new(3, 40, "Momo", "route du truc");
	p[1] = person_new(9, 100, "Momo", "route du truc");


	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		array_append(a, p[i]);
	}
	array_write(a, stdout); puts("-----------------");

	printf("sorting :\n");
	array_sort(a);
	array_write(a, stdout); puts("-----------------");

	array_free(a, 1);

	return 0;
}
