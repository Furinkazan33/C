#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "str.h"

int test_insert() {
	puts("==== test_insert ====");

	char *s1, *s2;
   
	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, 4, s2);
	printf("%s\n", s1);

	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, 0, s2);
	printf("%s\n", s1);

	s1 = malloc(sizeof(char) * 10); strcpy(s1, "123456789");
	s2 = malloc(sizeof(char) * 10); strcpy(s2, "abcdefghi");
	str_insert(s1, strlen(s1), s2);
	printf("%s\n", s1);


	free(s1);
	free(s2);

	return 0;
}

int test_shift() {
	puts("==== test_shift ====");

	char *s = malloc(sizeof(char) * 10);
	strcpy(s, "123456789");
	printf("%s\n", s);

	puts("shifting left");
	str_shift(s, 0, -5);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 3, -5);
	printf("%s\n", s);

	puts("shifting right");
	strcpy(s, "123456789");
	str_shift(s, 0, 0);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 0, 3);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 0, 9);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 3, 0);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 3, 2);
	printf("%s\n", s);

	strcpy(s, "123456789");
	str_shift(s, 3, 5);
	printf("%s\n", s);

	free(s);

	return 0;
}

int test_move() {
	puts("==== test_move ====");

	char *s = malloc(sizeof(char) * 256); strcpy(s, "un    oiseau    bleu     et jaune s'envole      \t vers le ciel.");
	printf("%s\n", s);

	char *p = str_next_blank(s);
	printf("%s\n", p);

	p = str_next_blank(p);
	printf("%s\n", p);

	p = str_next_non_blank(p);
	printf("%s\n", p);

	p = str_next_non_blank(p);
	printf("%s\n", p);

	p = str_end(p);
	printf("%s\n", p);

	p = str_next_blank(p);
	p = str_end(p);
	printf("%s\n", p);

	p = str_begin(s, p);
	printf("%s\n", p);

	p = str_next_non_blank(p);
	p = str_begin(s, ++p);
	printf("%s\n", p);

	p = str_next_word(p);
	printf("%s\n", p);

	p = str_next_blank(p);
	p = str_next_word(p);
	printf("%s\n", p);

	return 0;
}


int main(int argc, char **argv) {
	if(argc != 0) { printf("usage : %s\n", argv[0]); }

	TEST_INIT(argv[0]);

	TEST(shift);
	TEST(insert);
	TEST(move);

	TEST_RESULTS();

	return 0;
}

