#include <stdio.h>
#include <fcntl.h>
#include "array.h"

int int_cmp(int *a, int *b) {
	return *a - *b;
}
void int_write(int *a, FILE *file) {
	fprintf(file, "%d\n", *a);
}
void int_free(int *a) {
	free(a);
}
int *int_copy(int *a) {
	int *res = malloc(sizeof(int));
	*res = *a;
	return res;
}

void map(int *a) {
	*a += 10;
}
void reduce(int *res, int *a) {
	*res += *a;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *a = array_new(10);
	array_set_cmp(a, (int (*)(void *, void *))int_cmp);
	array_set_free(a, (void (*)(void *))int_free);
	array_set_write(a, (void (*)(void *, FILE *))int_write);

	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		int *n = malloc(sizeof(int));
		*n = i;
		array_append(a, n);
	}
	array_write(a, stdout);
	puts("-----------------");

	printf("map +10 :\n");
	array_map(a, (void (*)(void *)) &map);
	array_write(a, stdout);
	puts("-----------------");

	int res = 0;
	array_reduce(a, &res, (void (*)(void *, void *))&reduce);
	printf("reduce : %d\n", res);
	array_write(a, stdout);
	puts("-----------------");

	array_free(a, 1);

	return 0;
}
