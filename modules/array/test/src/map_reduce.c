#include <stdio.h>
#include <fcntl.h>
#include "array.h"

int int_cmp(void *a, void *b) {
	return *(int *)a - *(int *)b;
}
void int_write(void *a, FILE *file) {
	fprintf(file, "%d\n", *(int *)a);
}
void int_free(void *a) {
	free((int *)a);
}
int *int_copy(int *a) {
	int *res = malloc(sizeof(int));
	*res = *a;
	return res;
}

void *map(void *a) {
	*(int *)a += 10;

	return a;
}
void reduce(void *res, void *a) {
	*(int *)res += *(int *)a;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}
	array *a = array_new(10);
	a->cmp = int_cmp;
	a->free = int_free;
	a->write = int_write;

	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		int *n = malloc(sizeof(int));
		*n = i;
		array_append(a, n);
	}
	array_write(a, stdout);
	puts("-----------------");

	printf("map +10 :\n");
	array_map(a, map);
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
