#include <stdio.h>
#include "config.h"


void usage(char *exec) {
	fprintf(stderr, "usage : %s\n", exec);
}

int main(int argc, char **argv) {
	if(argc != 1) {
		usage(argv[0]);
		return 1;
	}

	config_read("config/init.cfg");

	config_print();

	config_write();

	return 0;

}

