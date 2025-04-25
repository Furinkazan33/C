#include "m33.h"

void m33_add(m33 *dest, m33 src) {
	for(size_t i = 0; i < 3; i++) {
		for(size_t j = 0; j < 3; j++) {
			(*dest)[i][j] += src[i][j];
		}
	}
}

void m33_add_to(m33 *dest, m33 m1, m33 m2) {
	for(size_t i = 0; i < 3; i++) {
		for(size_t j = 0; j < 3; j++) {
			(*dest)[i][j] = m1[i][j] + m2[i][j];
		}
	}
}

void m33_print(m33 m) {
	for(size_t i = 0; i < 3; i++) {
		for(size_t j = 0; j < 3; j++) {
			printf("%f ", m[i][j]);
		}
		printf("\n");
	}
}



