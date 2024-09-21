#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <helpers.h>
#include "str.h"


void test_insert() {
	str *s1 = str_new("un chat gris.");
	str *i = str_new("petit ");
	str *expected = str_new("un petit chat gris.");

	str_write(s1, stdout);
	str_write(i, stdout);
	str_inserts(s1, 3, i);
	str_write(s1, stdout);

	assert(!str_cmp(s1, expected));

	str_free(s1);
	str_free(i);
	str_free(expected);
}

void test_cat() {
	str *s = str_new("");
	str *cat = str_new("0123456789");

	str_write(s, stdout);
	str_write(cat, stdout);

	str_cats(s, cat);

	str_write(s, stdout);

	assert(!str_cmp(s, cat));

	str_free(s);
	str_free(cat);
}

void test_replace() {
	str *s = str_new("");
	str_write(s, stdout);
	str_replace(s, "123");
	str_write(s, stdout);
	assert(!strcmp(s->data, "123"));
	str_free(s);
}

void test_tokenize(char *src) {
	str *s = str_new(src);
	char **tokens = str_tokenize(s);

	str_write(s, stdout);
	for(size_t i = 0; tokens[i]; i++) {
		printf("[%s]", tokens[i]);
	}
	puts("");
	
	for(size_t i = 0; tokens[i]; i++) {
		free(tokens[i]);
	}
	free(tokens);
	str_free(s);
}

void test_escape_normalize() {
	char *string = "0\1\t234\\@56789";
	str *s = str_new(string);

	str_write(s, stdout); fflush(stdout);
	str_escape(s, "\\@", '\\');
	str_write(s, stdout); fflush(stdout);
	str_normalize(s, '\\');
	str_write(s, stdout); fflush(stdout);

	assert(!strcmp(s->data, string));
	str_free(s);
}

int main(void) {
	TEST(insert);
	TEST(cat);
	TEST(replace);
	TEST(escape_normalize);
	TESTA(tokenize, "un petit chat noir.");
	TESTA(tokenize, ".");
	TESTA(tokenize, "");

	fprintf(stdout, "test1 : test OK\n");

	return 0;
}
