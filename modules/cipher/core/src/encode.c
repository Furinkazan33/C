#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cipher.h"


int main(int argc, char **argv) {
	char *to_encode = "abcdefgh";
	char decoded[1024] = { 0 };
	char encoded[1024] = { 0 };
	int base;
	int n_bits;

	if(argc < 2) {
		printf("usage : %s base\n", argv[0]);
		return 1;
	}

	base = atoi(argv[1]);
	assert(base >= 2 && base < 98);

	printf("encoding:%s\n", to_encode);

	encode(encoded, to_encode, base, &n_bits);
	printf("coded  (%d bits):%s\n", n_bits, encoded);

	decode(decoded, encoded, base, &n_bits);
	printf("decoded(%d bits):%s\n", n_bits, decoded);

	return 0;
}
