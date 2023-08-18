#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "db.h"

#define INIT_ALLOC_STRING 2

int main(void) {
	db_base *db = db_new(1, 6);

	/* adding columns */
	db_add_col(db, INTEGER);

	/* insertions par valeur */
	void **l0 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l0, 0, 0);
	db_insert(db, l0, 1);
	db_print(db);

	/* adding columns */
	db_add_col(db, STRING);
	db_print(db);

	/* insertions par valeur */
	void **l1 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l1, 0, 1);
	db_line_set_string(db, l1, 1, "test 1");
	db_insert(db, l1, 2);
	db_print(db);

	void **l2 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l2, 0, 2);
	db_line_set_string(db, l2, 1, "test 2");
	db_insert(db, l2, 2);

	void **l3 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l3, 0, 3);
	db_line_set_string(db, l3, 1, "test 3");
	db_insert(db, l3, 2);

	void **l4 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l4, 0, 4);
	db_line_set_string(db, l4, 1, "test 4");
	db_insert(db, l4, 2);

	void **l5 = db_line_new(db, 1, INIT_ALLOC_STRING);
	db_line_set_int(db, l5, 0, 5);
	db_line_set_string(db, l5, 1, "test 5");
	db_insert(db, l5, 2);

	/* insertion par reference */
	void **l6 = db_line_new(db, 0, INIT_ALLOC_STRING);
	int *i = malloc(sizeof(int));
	*i = 6;
	db_line_set_byref(db, l6, 0, INTEGER, i, 0);
	char *s = malloc(sizeof(char) * 10);
	strcpy(s, "test 6");
	db_line_set_byref(db, l6, 1, STRING, s, 10);
	db_insert(db, l6, 2);
	db_print(db);

	printf("Deleting 4 & 5, keeping nulls\n");
	db_delete(db, 4, 1);
	db_delete(db, 5, 1);
	db_print(db);

	printf("Removing nulls\n");
	db_remove_nulls(db);
	db_print(db);

	printf("Resizing base :\n");
	db_resize(db, 0.6, 0.7);
	db_print(db);

	db_free(db);

	return 0;
}
