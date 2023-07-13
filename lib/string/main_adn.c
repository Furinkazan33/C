#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef enum adn { T = 0, A, G, C } letter;

char adntob4(char c) {
	char res;

	switch(c) {
		case 'T':
		res = '0';
		break;

		case 'A':
		res = '1';
		break;

		case 'G':
		res = '2';
		break;

		case 'C':
		res = '3';
		break;
	}
	return res;
}

char base4toadn(char c) {
	char res;

	switch(c) {
		case '0':
		res = 'T';
		break;

		case '1':
		res = 'A';
		break;

		case '2':
		res = 'G';
		break;

		case '3':
		res = 'C';
		break;
	}
	return res;
}

char *adn_to_base4(char *adn) {
	char *res = malloc(sizeof(char) * strlen(adn));

	for(size_t i = 0; i < strlen(adn); i++) {
		res[i] = adntob4(adn[i]);
	}

	return res;
}

char *base4_to_adn(char *base4) {
	char *res = malloc(sizeof(char) * strlen(base4));

	for(size_t i = 0; i < strlen(base4); i++) {
		res[i] = base4toadn(base4[i]);
	}

	return res;
}

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
	int c;

	for (int i = 0; i < n; i++) {
		c = *(word+i) - 48; /* ascii to int */

		assert(c >= 0 && c < base);

		if(c > 0) {
			res += c * my_pow(base, n-1-i);
		}
	}
	return res;
}

char *decimal_to_base(int decimal, int base) {
	char *res;
	int n_bits;

	/* alloc length of result */
	for(n_bits = 1; my_pow(base, n_bits) <= decimal; n_bits++) {}
   	res = malloc(sizeof(char) * (n_bits + 1));
	res[n_bits] = '\0';
	n_bits--;

	if(decimal == 0) {
		res[n_bits] = '0';
		return res;
	}

	while(decimal != 0) {
		res[n_bits--] = decimal % base + 48; /* decimal to ascii */
		decimal /= base;
	}

	return res;
}

void decodage(char *dest, char *string[], int base) {
	for(int i = 0; string[i] != NULL; i++) {
		dest[i] = base_to_decimal(string[i], base);
	}
}

int main(void) {
	char *string = "Il fait beau aujourd'hui";
	char *encodage[512];
	char *encodage_adn[512];
	size_t i;

	printf("Input :\n%s\n", string);

	for(i = 0; i < strlen(string); i++) {
		encodage[i] = decimal_to_base(string[i], 4);
		encodage_adn[i] = base4_to_adn(encodage[i]);
	}
	encodage[i] = NULL;

	printf("En base ADN : \n");
	for(i = 0; encodage_adn[i] != NULL; i++) {
		printf("%s", encodage_adn[i]);
	}
	puts("");

	for(i = 0; encodage_adn[i] != NULL; i++) {
    	encodage[i] = adn_to_base4(encodage_adn[i]);
	}
	encodage[i] = NULL;

	printf("Decodage en clair :\n");
	for(i = 0; encodage[i] != NULL; i++) {
		printf("%c", base_to_decimal(encodage[i], 4));
	}
	puts("");


	for(i = 0; encodage[i] != NULL; i++) {
		free(encodage[i]);
		free(encodage_adn[i]);
	}
}


