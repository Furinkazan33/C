#include <stdio.h>
#include <fcntl.h>
#include "array.h"
#include "person.h"

#define INIT_SIZE 4


void *transform(void *p) {
	int *id = malloc(sizeof(int));
	*id = ((person *)p)->id;

	person_free((person *)p);

	return id;
}

void write_int(void *i, FILE *file) {
	fprintf(file, "%d\n", *(int *)i);
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *a = array_new(INIT_SIZE);
	if(!a) {
		return 1;
	}
	a->cmp = person_cmp;
	a->free = person_free;
	a->write = person_write;

	person *p[5];
	p[0] = person_new(0, 10, "Momo", "route du truc");
	p[1] = person_new(1, 20, "Momo", "route du truc");
	p[2] = person_new(2, 30, "Momo", "route du truc");
	p[3] = person_new(3, 40, "Momo", "route du truc");
	p[4] = person_new(4, 50, "Momo", "route du truc");

	for(int i = 0; i < 5; i++) {
		array_append(a, p[i]);
	}
	array_write(a, stdout);
	puts("-----------------");

	array *a2 = array_copy(a, person_copy); if(!a2) { return 1; }
	array_map(a2, transform);
	a2->write = write_int;
	a2->free = free;
	array_write(a2, stdout);
	puts("-----------------");


	array_free(a, 1);
	array_free(a2, 1);

	return 0;
}
