#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "macros.h"


int test_1() {
	char line[256];
	size_t len;
	
	while(*readline(stdin, &line[0], &len)) {
		char **args = tokenize(line, strlen(line) - 1);
		
		#ifdef DEBUG
			for (char **a = args; a && *a; a++) {
				LOG_S("[%s][%s]\n", type_to_s(get_type(*a, strlen(*a))), *a);
			}
		#endif
		
		for (char **a = args; a && *a; a++) {
			free(*a);
		}
		free(args);
	}

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	return test_1();
}
