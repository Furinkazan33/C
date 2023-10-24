#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"

#define INIT_COLS_ALLOC 3
#define INIT_LINES_ALLOC 3

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "usage : %s file\n", argv[0]);
		return 1;
	}
	FILE *file = fopen(argv[1], "w");

	array *line;
	char tmp[20];
	int *i;
	char *s;

	db_base *db = db_new_with_id(INIT_COLS_ALLOC, INIT_LINES_ALLOC, "test_create", "creation test");

	db_col_add(db, false, false, STRING, "name", "name of person");

	for(size_t l = 1; l < 8; l++) {
		s = malloc(sizeof(char) * 7);
		sprintf(tmp, "test %ld", l);
		strcpy(s, tmp);
		line = db_line_new_with_id(db);
		db_line_set(db, line, 1, STRING, s, 0);
		db_insert(db, line);
	}

	db_col_add(db, false, false, INTEGER, "age", "age of person");
	db_col_add(db, false, false, STRING, "address", "address of person");
	db_col_add(db, false, false, DOUBLE, "salary", "salary of person");
	db_col_add(db, true, true, STRING, "email", "email of person"); //TODO: check required fields in src

	line = db_line_new_with_id(db);
	s = malloc(sizeof(char) * 7);
	strcpy(s, "test 8");
	db_line_set(db, line, 1, STRING, s, 0);
	i = malloc(sizeof(int));
	*i = 25;
	db_line_set(db, line, 2, INTEGER, i, 0);
	s = malloc(sizeof(char) * 63);
	strcpy(s, "22, rue de la chose - 33700 - Merignac");
	db_line_set(db, line, 3, STRING, s, 63);
	double *d = malloc(sizeof(double));
	*d = 601234.13;
	db_line_set(db, line, 4, DOUBLE, d, 0);
	db_insert(db, line);

	db_write(db, file);

	db_free(db);

	return 0;
}
