#include <stdlib.h>
#include <stdio.h>
#include "m33.h"

int test_1() {
	puts("");

	m33 m1 = {
		{ 0, 0, 0 },
		{ 0, 0, 0 },
		{ 0, 0, 0 },
	};
	m33 m2 = {
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 },
	};

	m33 res;
	m33_add_to(&res, m1, m2);
	m33_print(res);


	m33_add(&m1, m2);
	m33_print(m1);

	return 0;
}

int test_2(char *src) {
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	TEST_INIT(argv[0]);
	TEST(1);
	TESTA(2, "test.");
	TEST_RESULTS();
}

