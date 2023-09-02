#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define _POSIX_C_SOURCE 200809L

#define SIZE 100
#define MIN 0
#define MAX 10000

uint32_t nLehmer;

void rnd_set_seed(uint32_t seed) {
	nLehmer = seed;
}

uint32_t __lehmer32() {
	uint64_t tmp;
	uint32_t m1, m2;

	nLehmer += 0xe120fc15;

	tmp = (uint64_t)nLehmer * 0x4a39b70d;
	m1 = (tmp >> 32) ^ tmp;
	tmp = (uint64_t)m1 * 0x12fad5c9;
	m2 = (tmp >> 32) ^ tmp;

	return m2;
}

int rnd_int(int min, int max) {
	return (__lehmer32() % (max - min)) + min;
}

double rnd_double(double min, double max) {
	return ((double)__lehmer32() / (double)(0x7FFFFFFF)) * (max - min) + min;
}


int result[SIZE];


int main(void) {
	int i;
	int n;

	rnd_set_seed(1);

	for(i = 0; i < SIZE; i++) {
		result[i] = rnd_int(MIN, MAX);
	}

	for(i = 0; i < SIZE; i++) {
		printf("%d ", result[i]);
	}

	return 0;
}

