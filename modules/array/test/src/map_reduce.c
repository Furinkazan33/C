#include <stdio.h>
#include <fcntl.h>
#include "array.h"

int int_cmp(int *a, int *b) {
	return *a - *b;
}
void int_print(int *a) {
	printf("%d\n", *a);
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
	array *a = array_new(10,\
					(int (*)(void *, void *))int_cmp,\
					(void (*)(void *))int_free,\
					(void (*)(void *))int_print);

	printf("Adding 10 :\n");
	for(int i = 0; i < 10; i++) {
		int *n = malloc(sizeof(int));
		*n = i;
		array_append(a, n);
	}
	array_print(a);
	puts("-----------------");

	printf("map +10 :\n");
	array_map(a, (void (*)(void *)) &map);
	array_print(a);
	puts("-----------------");

	int res = 0;
	array_reduce(a, &res, (void (*)(void *, void *))&reduce);
	printf("reduce : %d\n", res);
	array_print(a);
	puts("-----------------");

	array_free(a, 1);

	return 0;
}
