#include <stdlib.h>
#include <stdio.h>
#include "myrand.h"

#define _POSIX_C_SOURCE 200809L

#define SIZE 100
#define MIN 0
#define MAX 10000

int result[SIZE];


int main(void) {
	int i;
	int n;

	rnd_set_seed(1);

	for (i = 0; i < SIZE; i++) {
		result[i] = rnd_int(MIN, MAX);
	}

	for (i = 0; i < SIZE; i++) {
		printf("%d ", result[i]);
	}

	return 0;
}

