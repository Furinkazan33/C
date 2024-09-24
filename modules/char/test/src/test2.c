#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "char.h"



void test2() {
	char *line = malloc(sizeof(char) * 128);
	strcpy(line, "un petit      oiseau bleu.");
	char *start = line;
	char *p = start;

	printf("newline=%s\n", line);

	puts("déplacement sur petit");
	p = char_eow_r(p);
	p = char_eow_r(p);
	p = char_bow_r(start, p);
	printf("%s \n", p);

	puts("daw");	
	char_daw(start, p);
	printf("%s \n", p);

	p = char_bow_r(start, p);
	p = char_bow_r(start, p);
	printf("%s \n", p);

	strcpy(line, "un petit      oiseau bleu.");
	p = &line[10];
	printf("newline=[%s], position on [%s]\n", line, p);
	puts("daw");
	char_daw(start, p);
	printf("%s \n", line);
	free(line);
}



int main(void) {

	test2();

	return 0;
}

