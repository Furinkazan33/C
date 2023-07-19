#include <stdio.h>
#include "config.h"


void usage(char *exec) {
	fprintf(stderr, "usage : %s filepath\n", exec);
}

int main(int argc, char **argv) {
	if(argc != 2) {
		usage(argv[0]);
		return 1;
	}

	config *conf = config_read(argv[1], "config");
	if(!conf) {
		return 1;
	}

	conf_print(conf);
	conf_free(conf);

	return 0;
}

