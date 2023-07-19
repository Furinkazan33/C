#include <stdlib.h>
#include <string.h>
#include "database.h"


int main(void) {
	any *row[2];
	database *db = db_new("un test", "un petit test de creation de db");
	db_add_col(db, db_new_col("ID", "id", sizeof(int)));
	db_add_col(db, db_new_col("NAME", "name", sizeof(char) * 10));

	/* insertions */
	char n[10] = "000";
	int i = 1;
	row[0] = malloc(sizeof(int));
	row[1] = malloc(sizeof(char) * 10);
	memcpy(row[0], &i, sizeof(int));
	memcpy(row[1], n, sizeof(char) * 10);

	db_insert(db, row);
	
/*
	i = 2;
	n[0] = 'X';
	memcpy(row, &i, sizeof(int));
	memcpy(row+1, n, sizeof(char) * 10);
	db_insert(db, row);

	i = 3;
	n[1] = 'X';
	memcpy(row, &i, sizeof(int));
	memcpy(row+1, n, sizeof(char) * 10);
	db_insert(db, row);

	i = 4;
	n[2] = 'X';
	memcpy(row, &i, sizeof(int));
	memcpy(row+1, n, sizeof(char) * 10);
	db_insert(db, row);


	db_print(db);

	db_free(db);
*/

	return 0;
}
