#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

#define DB_ALLOC_ROWS 500
#define DB_ALLOC_COLS 2

#define DB_NAME_LEN 128
#define DB_COMMENT_LEN 256

#define any uint8_t

typedef struct col {
	char name[DB_NAME_LEN];
	char desc[DB_COMMENT_LEN];
	size_t data_size;
} col;

typedef struct database {
	char name[DB_NAME_LEN];
	char desc[DB_COMMENT_LEN];

	size_t n_cols;
	size_t max_cols;
	col **cols;

	size_t n_rows;
	size_t max_rows;
	any ***data;
} database;

database *db_new(char *name, char *desc) {
	database *db = (database *)malloc(sizeof(database));
	if(!db) {
		perror("");
		return NULL;
	}
	db->cols = (col **)malloc(sizeof(col *) * DB_ALLOC_COLS);
	if(!db->cols) {
		free(db);
		perror("");
		return NULL;
	}
	db->data = (any ***)malloc(sizeof(any **) * DB_ALLOC_ROWS);
	if(!db->data) {
		perror("");
		return NULL;
	}

	strcpy(db->name, name);
	strcpy(db->desc, desc);
	db->cols[0] = NULL;
	db->data[0][0] = NULL;
	db->n_rows = 0;
	db->max_rows = DB_ALLOC_ROWS;
	db->n_cols = 0;
	db->max_cols = DB_ALLOC_COLS;

	return db;
}

col *db_new_col(char *name, char *desc, size_t data_size) {
	col *res = (col *)malloc(sizeof(col));

	strcpy(res->name, name);
	strcpy(res->desc, desc);
	res->data_size = data_size;

	return res;
}

col **db_realloc_cols(database *db) {
	col **tmp;

	if(db->n_cols >= db->max_cols) {
		tmp = (col **)realloc(db->cols, sizeof(col *) * db->max_cols * 2);
		if(!tmp) {
			perror("");
			return NULL;
		}
		db->cols = tmp;
		db->max_cols *= 2;
	}
	return db->cols;
}

any **db_realloc_row(database *db, int i) {
	db->data[i] = (any **)realloc(db->data[i], sizeof(any *) * db->n_cols);
	if(!db->data[i]) {
		perror("");
		return NULL;
	}
	return db->data[i];
}

any ***db_realloc_rows(database *db) {
	any ***tmp;

	if(db->n_rows < db->max_rows) {
		tmp = (any ***)realloc(db->data, db->max_rows * 2);
		if(!tmp) {
			perror("");
			return NULL;
		}
		db->data = tmp;
		db->max_rows *= 2;
	}
	return db->data;
}

col *db_add_col(database *db, col *c) {
	/* realloc if needed */
	if(!db_realloc_cols(db)) { return NULL; }

	db->cols[db->n_cols] = c;
	db->n_cols++;

	/* realloc every rows */
	for(size_t i = 0; i < db->n_rows; i++) {
		if(!db_realloc_row(db, i)) { return NULL; }
	}

	return c;
}

any **db_alloc_row(database *db, int n) {
	db->data[n] = (any **)malloc(sizeof(any *) * db->n_cols);
	if(!db->data[n]) {
		perror("");
		return NULL;
	}
	return db->data[n];
}

any *db_alloc_data(database *db, int row, int col) {
	db->data[row][col] = (any *)malloc(db->cols[col]->data_size);
	if(!db->data[row][col]) {
		perror("");
		return NULL;
	}

	return db->data[row][col];
}

any **db_insert(database *db, any *rows[]) {
	if(!db_realloc_rows(db)) { return NULL; }

	db->data[db->n_rows] = db_alloc_row(db, db->n_rows);
	if(!db->data[db->n_rows]) { return NULL; }

	/* alloc new datas on new row */
	for(size_t i=0; i<db->n_cols; i++) {
		db->data[db->n_rows][i] = db_alloc_data(db, db->n_rows, i);
		if(!db->data[db->n_rows][i]) { return NULL; }
		memcpy(db->data[db->n_rows][i], rows+i, db->cols[i]->data_size);
	}

	db->n_rows++;

	return db->data[db->n_rows - 1];
}

void db_free_cols(database *db) {
	for(size_t i = 0; i < db->n_cols; i++) {
		free(db->cols[i]);
	}
}

void db_free_row(database *db, int n) {
	for(size_t i = 0; i < db->n_cols; i++) {
		free(db->data[n][i]);
	}
}

void db_delete(database *db, int n) {
	db_free_row(db, n);
	db->n_rows--;

	for(size_t r = n; r < db->n_rows; r++) {
		for(size_t i = 0; i < db->n_cols; i++) {
			db->data[r][i] = db->data[r+1][i];
		}
	}
}

void db_free_rows(database *db) {
	for(size_t i = 0; i < db->n_rows; i++) {
		db_free_row(db, i);
	}
}

void db_free(database *db) {
	db_free_rows(db);
	db_free_cols(db);
	free(db);
}

void db_print_stats(database *db) {
	printf("DB name : %s (%ld rows for %ld cols)\n", db->name, db->n_rows, db->n_cols);
}

void db_print_desc(database *db) {
	for(size_t i=0; i<db->n_cols; i++) {
		printf("%s\t", db->cols[i]->desc);
	}
	printf("\n");
}

void db_print(database *db) {
	db_print_stats(db);
	db_print_desc(db);
}

