#include <stdlib.h>
#include <string.h>
#include "database.h"


int main(void) {
	void *row[DB_N_COLUMNS];
	database *db = db_new("un test", "un petit test de creation de db");
	db_add_col(db, db_new_col("ID", "id", &db_t_int));
	db_add_col(db, db_new_col("NAME", "name", &db_t_string));

	/* insertions */
	char n[10] = "000";
	int i = 1;
	row[0] = &i;
	row[1] = &n[0];

	db_insert(db, row);

	i = 2;
	n[0] = 'X';
	db_insert(db, row);

	i = 3;
	n[1] = 'X';
	db_insert(db, row);

	i = 4;
	n[2] = 'X';
	db_insert(db, row);


	db_print(db);


	db_free(db);


	return 0;
}
