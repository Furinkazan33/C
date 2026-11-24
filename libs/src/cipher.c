#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cipher.h"


char *fill_left(char *src, size_t n) {
	assert(n > strlen(src));
	
	char *res = malloc(n + 1);
	for(size_t i = 0; i < n - strlen(src); i++) {
		res[i] = '0';
	}
	strcpy(res + n - strlen(src), src);
	
	return res;
}


int _my_pow(int a, int b) {
	int res = 1;

	for(int i = 0; i < b; i++) {
		res *= a;
	}
	return res;
}

int base_to_decimal(char *word, int base) {
	int res = 0;
	size_t n = strlen(word);
	int c;

	for(size_t i = 0; i < n; i++) {
		c = word[i] - 48; /* char to int */

		assert(c >= 0 && c < base);

		if(c > 0) {
			res += c * _my_pow(base, n - 1 - i);
		}
	}
	
	return res;
}

char *decimal_to_base(int decimal, int base) {
	int d = decimal;
	size_t nbits = 0;
	while(d > 0) {
		nbits++;
		d /= base;
	}
	
	char *res = malloc(nbits + 1);
	res[nbits] = 0;

	for(--nbits; decimal > 0; nbits--) {
		res[nbits] = decimal % base + 48; /* decimal to ascii */
		decimal /= base;
	}

	return res;
}

char *base_to_base(char *word, int from, int to) {
	int dec = base_to_decimal(word, from);

	return decimal_to_base(dec, to);
}

