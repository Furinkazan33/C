#include <string.h>
#include <stdio.h>

static void usage(int argc, char **argv, char *mess) {
	int i;

	if(strlen(mess) > 0) {
		fprintf(stderr, "%s\n", mess);
	}

	fprintf(stderr, "[%d]:[", argc - 1);

	if(argc > 1) {
		for(i = 1; i < argc - 1; i++) {
			fprintf(stderr, "%s ", argv[i]);
		}
		fprintf(stderr, "%s", argv[i]);
	}
	fprintf(stderr, "]\n");

	fprintf(stderr, "usage: %s\n", argv[0]);
}

