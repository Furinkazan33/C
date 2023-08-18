#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define DB_MAX_ROWS 500
#define DB_N_COLUMNS 2
#define DB_NAME_LEN 128
#define DB_COMMENT_LEN 256

typedef enum type {
	INT = 0, 	DOUBLE, 	FLOAT, 		CHAR, 		STRING,
} type;

const char *type_to_s[] = {
	"INT", 		"DOUBLE", 	"FLOAT", 	"CHAR", 	"STRING",
};

const type db_t_int = INT;
const type db_t_double = DOUBLE;
const type db_t_float = FLOAT;
const type db_t_char = CHAR;
const type db_t_string = STRING;

typedef struct col {
	char name[DB_NAME_LEN];
	const type *type;
	char desc[DB_COMMENT_LEN];
} col;

typedef struct database {
	char name[DB_NAME_LEN];
	char desc[DB_COMMENT_LEN];

	col *cols[DB_N_COLUMNS];

	void *data[DB_MAX_ROWS][DB_N_COLUMNS];

	int n_rows;
	int n_cols;
} database;

database *db_new(char *name, char *desc) {
	database *db = (database *)malloc(sizeof(database));
	strcpy(db->name, name);
	strcpy(db->desc, desc);
	db->cols[0] = NULL;
	db->data[0][0] = NULL;
	db->n_rows = 0;
	db->n_cols = 0;

	return db;
}

col *db_new_col(char *name, char *desc, const type *t) {
	col *res = (col *)malloc(sizeof(col));

	strcpy(res->name, name);
	strcpy(res->desc, desc);
	res->type = t;

	return res;
}

void db_add_col(database *db, col *c) {
	assert(db->n_cols < DB_N_COLUMNS);

	db->cols[db->n_cols] = c;
	db->n_cols++;
}

void db_insert(database *db, void *row[DB_N_COLUMNS]) {
	assert(db->n_rows < DB_MAX_ROWS);
	int *p_i;
	char *p_c;

	for(int i=0; i<db->n_cols; i++) {

		switch(*(db->cols[i]->type)) {
			case INT:
			p_i = (int *)malloc(sizeof(int));
			*p_i = *(int *)row[i];
			db->data[db->n_rows][i] = p_i;
			break;

			case STRING:
			p_c = (char *)malloc(sizeof(char) * strlen((char *)row[i]));
			strcpy(p_c, (char *)row[i]);
			db->data[db->n_rows][i] = p_c;
			break;

			default:
			break;
		}
	}

	db->n_rows++;
}
void db_free_cols(database *db) {
	for(int i=0; i<db->n_cols; i++) {
		free(db->cols[i]);
	}
}

void db_free_data(database *db, int n) {
	for(int i=0; i<db->n_cols; i++) {
		free(db->data[n][i]);
	}
}

void db_delete(database *db, int n) {
	db_free_data(db, n);

	while(n < DB_MAX_ROWS - 1 && n < db->n_rows) {
		for(int i=0; db->data[n][i] != NULL && i<DB_N_COLUMNS; i++) {
			db->data[n][i] = db->data[n+1][i];
		}
		n++;
	}
	db->n_rows--;
}

void db_delete_all(database *db) {
	for(int i=0; i<db->n_rows; i++) {
		db_delete(db, i);
	}
}

void db_free(database *db) {
	db_delete_all(db);
	db_free_cols(db);
	free(db);
}

void db_print_stats(database *db) {
	printf("DB name : %s (%d rows for %d cols)\n", db->name, db->n_rows, db->n_cols);
}

void db_print_desc(database *db) {
	for(int i=0; i<db->n_cols; i++) {
		printf("%s\t", db->cols[i]->desc);
	}
	printf("\n");
}

void db_print_types(database *db) {
	int t;
	for(int i=0; i<db->n_cols; i++) {
		t = *(db->cols[i]->type);
		printf("%s\t", type_to_s[t]);
	}
	printf("\n");
}

void db_print_row(database *db, int n) {
	assert(n < db->n_rows);
	int t;

	for(int i=0; i<db->n_cols; i++) {
		t = *(db->cols[i]->type);
		switch(t) {
			case INT:
			printf("%d\t",  *((int *)(db->data[n][i])));
			break;

			case STRING:
			printf("%s\t", (char *)(db->data[n][i]));
			break;

			default:
			puts("TODO");
			break;
		}
	}
	printf("\n");

}

void db_print_rows(database *db) {
	for(int i=0; i<db->n_rows; i++) {
		db_print_row(db, i);
	}
}

void db_print(database *db) {
	db_print_stats(db);
	db_print_desc(db);
	db_print_types(db);
	db_print_rows(db);
}

