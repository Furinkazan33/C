#include <stdio.h>
#include <stdlib.h>
#include "config.h"


void usage(char *exec) {
	fprintf(stderr, "usage : %s load_filepath save_filepath\n", exec);
	exit(1);
}

int main(int argc, char **argv) {
	if(argc != 3) {
		usage(argv[0]);
	}

	cJSON *conf = config_new(argv[1]);
	if(!conf) {
		return 1;
	}

	config_save(conf, argv[2]);

	cJSON_Delete(conf);

	return 0;
}

