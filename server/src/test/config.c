#include <stdio.h>
#include "../include/config.h"


void usage(char *exec) {
	fprintf(stderr, "usage : %s\n", exec);
}

int main(int argc, char **argv) {
	if(argc != 1) {
		usage(argv[0]);
		return 1;
	}

	config *conf = config_read("config/init.cfg", "config");

	if(conf) {
		conf_print(conf);

		conf_free(conf);
	}
	else {
		return 1;
	}

	return 0;
}

