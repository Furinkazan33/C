#include <stdio.h>
#include <fcntl.h>
#include "array.h"
#include "person.h"

#define INIT_SIZE 10

int person_cmp_name(void *p1, void *p2) {
	return strcmp(((person *)p1)->name, ((person *)p2)->name);
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *a = array_new(INIT_SIZE);
	array_set_free(a, (void (*)(void *))person_free);
	array_set_write(a, (void (*)(void *, FILE *))person_write);


	person *p[10];
	p[8] = person_new(0, 10, "Mama", "route du truc");
	p[7] = person_new(1, 20, "Mimi", "route du truc");
	p[5] = person_new(2, 30, "Momo", "route du truc");
	p[6] = person_new(4, 50, "Tata", "route du truc");
	p[9] = person_new(5, 60, "Momo", "route du truc");
	p[3] = person_new(6, 70, "Titi", "route du truc");
	p[4] = person_new(7, 80, "Momo", "route du truc");
	p[2] = person_new(8, 90, "Tata", "route du truc");
	p[0] = person_new(3, 40, "Momo", "route du truc");
	p[1] = person_new(9, 100, "Tata", "route du truc");


	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		array_append(a, p[i]);
	}
	array_write(a, stdout); 
	puts("-----------------");

	array_set_cmp (a, person_cmp_name);
	array *find = array_find_all(a, p[6], 5);
	array_set_write(find, (void (*)(void *, FILE *))person_write);
	array_set_free(find, (void (*)(void *))person_free);
	array_write(find, stdout);
	array_free(find, 0);
	puts("-----------------");

	printf("sorting :\n");
	array_set_cmp (a, (int (*)(void *, void *))person_cmp);
	array_sort(a);
	array_write(a, stdout); 
	puts("-----------------");
	
	array_set_cmp (a, person_cmp_name);
	array *find2 = array_find_all(a, p[6], 5);
	array_set_write(find2, (void (*)(void *, FILE *))person_write);
	array_set_free(find2, (void (*)(void *))person_free);
	array_write(find2, stdout);
	array_free(find2, 0);
	puts("-----------------");

	array_free(a, 1);

	return 0;
}
