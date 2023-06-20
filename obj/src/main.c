#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "object.h"


int main(int argc, char **argv) {
	char line[LINE_MAX_LEN];
	char tokens[LINE_MAX_TOK][TOK_MAX_LEN];
	object o, *res;

	if(argc != 2) {
		printf("usage : %s path_to_file\n", argv[0]);
		exit(1);
	}

	object_init(&o);
	res = object_load(&o, argv[1], line, tokens);

	if(!res) {
		exit(1);
	}

	char path[50];
	strcpy(path, o.name);
	strcpy(path + strlen(o.name), ".obj.sav");

	res = object_save(&o, path);

	if(!res) {
		exit(1);
	}

	return 0;
}


