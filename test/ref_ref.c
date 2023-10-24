#include <stdio.h>
#include <stdlib.h>

int shift2(char **p) {
	(*p)++;
}

int shift(char **p) {
	(*p)++;

	shift2(p);
}


int main(void) {
	char *test = "123456789";

	shift(&test);

	printf("%s\n", test);

	return 0;
}
