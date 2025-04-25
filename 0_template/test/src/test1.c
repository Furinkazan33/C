#include <stdlib.h>
#include <stdio.h>
#include "XXXX.h"

int test_1() {
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

