#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_ASCII 256

int my_pow(int a, int b) {
	int res = 1;

	for(int i = 0; i < b; i++) {
		res *= a;
	}
	return res;
}

int base_to_decimal(char *word, int base) {
	int res = 0;
	int n = strlen(word);
	int c, i;

	for (i = 0; i < n; i++) {
		c = *(word+i) - 48; /* char to int */

		assert(c >= 0 && c < base);

		if(c > 0) {
			res += c * my_pow(base, n-1-i);
		}
	}
	return res;
}

char *decimal_to_base(int decimal, int base, int n_bits) {
	char *res;

   	res = malloc(sizeof(char) * (n_bits + 1));
	res[n_bits] = '\0';
	n_bits--;

	while(n_bits >= 0) {
		res[n_bits--] = decimal % base + 48; /* decimal to ascii */
		decimal /= base;
	}

	return res;
}

char *base_to_base(char *word, int from, int to) {
	int dec = base_to_decimal(word, from);

	return decimal_to_base(dec, to, 8);
}

void encode(char *dest, char *text, int base, int *n_bits) {
	*n_bits = 1;
	while(my_pow(base, *n_bits) < MAX_ASCII) {
		(*n_bits)++;
	}

	for(size_t i = 0; i < strlen(text); i++) {
		strcat(dest, decimal_to_base(text[i], base, *n_bits));
	}
}

void decode(char *dest, char *text, int base, int *n_bits) {
	*n_bits = 1;
	while(my_pow(base, *n_bits) < MAX_ASCII) {
		(*n_bits)++;
	}
	char word[*n_bits];

	for(size_t i = 0; i < strlen(text) - *n_bits; i++) {
		strncpy(word, text+(i * *n_bits), *n_bits);
		word[*n_bits] = '\0';
		dest[i] = base_to_decimal(word, base);
	}
}



