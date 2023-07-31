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

	cJSON *conf = config_new(argv[1]);
	if(!conf) {
		return 1;
	}

	config_save(conf, argv[1]);

	cJSON_Delete(conf);

	return 0;
}

