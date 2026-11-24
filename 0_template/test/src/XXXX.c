#include <stdlib.h>
#include <stdio.h>
#include "XXXX.h"

int test_1() {
	return 0;
}

int test_2(char *src) {
	(void) src;
	return 0;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		LOG_E("usage : %s\n", argv[0])
		return 1;
	}

	test_1();

	test_2("");

	return 0;
}


