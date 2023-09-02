#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"

#define INIT_COLS_ALLOC 3
#define INIT_LINES_ALLOC 3

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	return 0;
}
