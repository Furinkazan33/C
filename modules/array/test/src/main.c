#include <stdio.h>
#include <fcntl.h>
#include "array.h"
#include "person.h"

#define INIT_SIZE 4


int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *l = array_new(INIT_SIZE);
	if(!l) {
		return 1;
	}
	l->cmp = person_cmp;
	array_sort(l);
	l->free = person_free;
	l->write = person_write;


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


	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		array_append(l, p[i]);
	}
	array_write(l, stdout);
	puts("-----------------");

	printf("Removing 1->4(keep null), 7, 8->9 (keep null) :\n");
	array_remove_idx(l, 1, 1);
	array_remove_idx(l, 2, 1);
	array_remove_idx(l, 3, 1);
	array_remove_idx(l, 4, 1);
	array_remove_idx(l, 7, 0);
	array_remove_idx(l, 7, 1);
	person_free(l->data[8]);
	array_remove_idx(l, 8, 1);
	array_write(l, stdout);
	puts("-----------------");

  	printf("Searching 5 :\n");
	person *found = array_find(l, p[5]);
	if(found) {
		printf("found : %d\n", found->id);
		puts("-----------------");
	}

	printf("Removing nulls :\n");
	array_remove_nulls(l);
	array_write(l, stdout);
	puts("-----------------");

	printf("Adding p[1], p[2], p[3], p[4], p[8] :\n");
	array_append(l, p[1]);
	array_append(l, p[2]);
	array_append(l, p[3]);
	array_append(l, p[4]);
	array_append(l, p[8]);
	array_write(l, stdout);
	puts("-----------------");

	//array_free(l, 1);

	return 0;
}
