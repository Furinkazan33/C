#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"


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

	db_base *db = db_new(5, 8);

	/* new int column */
	db_col_add(db, true, INTEGER, "int", "id of line");

	i = malloc(sizeof(int));
	*i = 0;
	line = db_line_new(db);
	db_line_set_byref(db, line, 0, INTEGER, i, 0);
	db_insert(db, line);

	/* new string column */
	db_col_add(db, false, STRING, "string", "name of line");

	for(size_t l = 1; l < 8; l++) {
		i = malloc(sizeof(int));
		*i = l;
		s = malloc(sizeof(char) * 7);
		sprintf(tmp, "test %ld", l);
		strcpy(s, tmp);
		line = db_line_new(db);
		db_line_set_byref(db, line, 0, INTEGER, i, 0);
		db_line_set_byref(db, line, 1, STRING, s, 0);
		db_insert(db, line);
	}

	db_file_write(db, file);

	db_free(db);

	return 0;
}
