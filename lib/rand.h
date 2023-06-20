#include <inttypes.h>

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


