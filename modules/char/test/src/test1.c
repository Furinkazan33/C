#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char.h"


void test1(char *line) {
	strcpy(line, "un petit oiseau bleu");
	char *start = line;
	char *end = line + strlen(line);
	char *p = start;

	puts("Test des limites start et end lors des deplacements");

	for(int i = 0; i < 2; i++) {
		p = char_bow_r(start, end, p);
		printf("%s \n", p);
	}

	for(int i = 0; i < 5; i++) {
		p = char_eow_r(start, end, p);
		printf("%s \n", p);
	}
}


int main(void) {
	char *line = malloc(sizeof(char) * 128);

	test1(line);

	return 0;
}

