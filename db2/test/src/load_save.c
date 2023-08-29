#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"


#define INIT_COLS_ALLOC 5
#define INIT_LINES_ALLOC 50

int main(int argc, char **argv) {
	if(argc != 3) {
		fprintf(stderr, "usage: %s file_to_load file_to_write", argv[0]);
		exit(1);
	}

	FILE *load = fopen(argv[1], "r");
	FILE *save = fopen(argv[2], "w");

	db_base *db = db_new(INIT_COLS_ALLOC, INIT_LINES_ALLOC);
	if(!db) { exit(1); }

	db = db_file_load(db, load, 64);
	if(!db) { exit(1); }

	db_write(db, save);

	db_free(db);

	return 0;
}

