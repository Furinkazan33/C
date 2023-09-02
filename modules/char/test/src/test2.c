#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char.h"



void test2(char *line) {
	strcpy(line, "un petit      oiseau bleu.");
	char *start = line;
	char *end = line + strlen(line);
	char *p = start;

	puts("Test de suppression d'un mot");

	printf("%s \n", p);
	p = char_eow_r(start, end, p);
	p = char_eow_r(start, end, p);
	p = char_bow_r(start, end, p);
	printf("%s \n", p);

	char_daw(start, end, p);
	printf("%s \n", p);

	p = char_bow_r(start, end, p);
	p = char_bow_r(start, end, p);
	printf("%s \n", p);

	strcpy(line, "un petit      oiseau bleu.");
	printf("%s \n", line);
	p = &line[10];
	char_daw(start, end, p);
	printf("%s \n", line);
}



int main(void) {
	char *line = malloc(sizeof(char) * 128);

	test2(line);

	return 0;
}

