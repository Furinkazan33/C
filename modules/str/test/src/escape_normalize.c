#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "str.h"



int main(void) {
	str *s = str_new(30);
	char *string = "01\t234\\@56789";
	str_replace(s, string);

	str_escape(s, "\\@", '\\');
	str_normalize(s, '\\');

	assert(!strcmp(s->data, string));

	str_free(s);

	fprintf(stdout, "escape_normalize : test OK\n");

	return 0;
}
