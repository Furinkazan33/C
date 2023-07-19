#include <stdio.h>
#include <stdlib.h>
#include "cursor.h"

int main(void) {
	char *line = "un petit oiseau bleu";
	cursor _c;
	cursor *c = &_c;
	cursor_init(c, 1, "cursor", 0, line, line, line + strlen(line) - 1);

	str_next_word(c->bol, &c->p, c->eol);
	str_next_word(c->bol, &c->p, c->eol);
	printf("%s\n", c->p);

	str_eow(c->bol, &c->p, c->eol);
	printf("%s\n", c->p);

	str_bow(c->bol, &c->p, c->eol);
	printf("%s\n", c->p);

	str_prev_word(c->bol, &c->p, c->eol);
	printf("%s\n", c->p);

	str_eol(&c->p, c->eol);
	printf("%s\n", c->p);

	return 0;
}

