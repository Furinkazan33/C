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

	printf("[%ld][%ld]\n%s\n", s->n, s->alloc_len, s->data);

	str_shift(s, 21, 37);

	printf("[%ld][%ld]\n%s\n", s->n, s->alloc_len, s->data);


	str_free(s);


	return 0;
}
