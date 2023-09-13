#include <stdlib.h>
#include <stdio.h>
#include "client.h"



int main(int argc, char **argv) {

	if(argc != 2) {
		fprintf(stderr, "usage : %s client_id\n", argv[0]);
		exit(1);
	}

	return launch_client(atoi(argv[1]));
}
