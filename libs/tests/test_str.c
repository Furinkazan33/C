#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "macros.h"
#include "str.h"

int test_shift() {
	#ifdef DEBUG
		puts("==== test_shift ====");
	#endif
	
	char *s = malloc(sizeof(char) * 10);
	strcpy(s, "123456789");
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	#ifdef DEBUG
		puts("shifting left");
	#endif
	str_shift(s, 0, -5);
	assert(!strcmp(s, "6789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 3, -5);
	assert(!strcmp(s, "1239"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	#ifdef DEBUG
		puts("shifting right");
	#endif
	strcpy(s, "123456789");
	str_shift(s, 0, 0);
	assert(!strcmp(s, "123456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 0, 3);
	assert(!strcmp(s, "123123456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 0, 9);
	assert(!strcmp(s, "123456789123456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 3, 0);
	assert(!strcmp(s, "123456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 3, 2);
	assert(!strcmp(s, "12345456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	strcpy(s, "123456789");
	str_shift(s, 3, 5);
	assert(!strcmp(s, "12345678456789"));
	#ifdef DEBUG
		printf("%s\n", s);
	#endif
	
	free(s);

	return 0;
}

int test_insert() {
	#ifdef DEBUG
		puts("==== test_insert ====");
	#endif
	
	char *s1, *s2;
   
	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, 4, s2);
	assert(!strcmp(s1, "1234abcdefghi56789"));
	#ifdef DEBUG
		printf("%s\n", s1);
	#endif

	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, 0, s2);
	assert(!strcmp(s1, "abcdefghi123456789"));
	#ifdef DEBUG
		printf("%s\n", s1);
	#endif

	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, strlen(s1), s2);
	assert(!strcmp(s1, "123456789abcdefghi"));
	#ifdef DEBUG
		printf("%s\n", s1);
	#endif


	free(s1);
	free(s2);

	return 0;
}



int test_move() {
	//puts("==== test_move ====");

	char *s = malloc(sizeof(char) * 256); strcpy(s, "un    oiseau    bleu     et jaune s'envole      \t vers le ciel.");
	//printf("%s\n", s);

	char *p = str_next_blank(s);
	assert(!strcmp(p, "    oiseau    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_blank(p);
	assert(!strcmp(p, "    oiseau    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_non_blank(p);
	assert(!strcmp(p, "oiseau    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_non_blank(p);
	assert(!strcmp(p, "oiseau    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_end(p);
	assert(!strcmp(p, "u    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_blank(p);
	p = str_end(p);
	assert(!strcmp(p, " bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_begin(s, p);
	assert(!strcmp(p, "    bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_non_blank(p);
	p = str_begin(s, ++p);
	assert(!strcmp(p, "bleu     et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_word(p);
	assert(!strcmp(p, "et jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	p = str_next_blank(p);
	p = str_next_word(p);
	assert(!strcmp(p, "jaune s'envole      \t vers le ciel."));
	//printf("%s\n", p);

	return 0;
}


int main(int argc, char **argv) {
	if(argc != 1) { printf("usage : %s\n", argv[0]); return 1; }

	test_shift();
	test_insert();
	test_move();


	return 0;
}

