#include <stdio.h>
#include <string.h>
#include "str.h"



int main(void) {
	str *s = str_new(10);

	char *to_cat = "0123456789 123456789 123456789\n";
	str_cat(s, to_cat, strlen(to_cat));
	str_cat(s, to_cat, strlen(to_cat));
	str_cat(s, to_cat, strlen(to_cat));
	str_cat(s, to_cat, strlen(to_cat));
	str_write(s, stdout);

	str_shift(s, 21, 37);
	str_write(s, stdout);

	str_replace(s, "123");
	str_write(s, stdout);

	char *string = malloc(sizeof(char) * 30);
	strcpy(string, "le petit chat");
	str_set(s, string, 30);
	str_write(s, stdout);

	str_free(s);


	fprintf(stdout, "test1 : test OK\n");

	return 0;
}
