#include <stdio.h>
#include <fcntl.h>
#include <sf.h>
#include "array.h"
#include <person.h>

#define INIT_SIZE 10


FUNC_SF(person)


int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "usage : %s json_file", argv[0]);
		exit(1);
	}
	array *l = array_new_sorted(person_sf(), INIT_SIZE);
	if(!l) {
		return 1;
	}

	person *p[10];
	p[0] = person_new(0, 10, "Momo", "route du truc");
	p[1] = person_new(1, 20, "Momo", "route du truc");
	p[2] = person_new(2, 30, "Momo", "route du truc");
	p[3] = person_new(3, 40, "Momo", "route du truc");
	p[4] = person_new(4, 50, "Momo", "route du truc");
	p[5] = person_new(5, 60, "Momo", "route du truc");
	p[6] = person_new(6, 70, "Momo", "route du truc");
	p[7] = person_new(7, 80, "Momo", "route du truc");
	p[8] = person_new(8, 90, "Momo", "route du truc");
	p[9] = person_new(9, 100, "Momo", "route du truc");


	for(int i = 0; i < 10; i++) {
		array_add(l, p[i]);
	}
	array_print(l);
	puts("-----------------");

	array_remove_keep_nulls(l, 1, 0);
	array_remove_keep_nulls(l, 2, 0);
	array_remove_keep_nulls(l, 3, 0);
	array_remove_keep_nulls(l, 4, 0);
	array_remove_keep_nulls(l, 5, 0);
	array_remove_keep_nulls(l, 6, 1);
	array_remove_keep_nulls(l, 7, 1);
	array_remove_keep_nulls(l, 8, 1);
	array_print(l);
	puts("-----------------");

	person *found = array_find(l, p[4]);
	if(found) {
		printf("found : %d\n", found->id);
		puts("-----------------");
	}

	array_remove_nulls(l, 1);
	array_print(l);
	puts("-----------------");

	array_add(l, p[1]);
	array_add(l, p[2]);
	array_add(l, p[3]);
	array_add(l, p[4]);
	array_add(l, p[5]);
	array_print(l);
	puts("-----------------");

	array_free(l, 1);

	return 0;
}
