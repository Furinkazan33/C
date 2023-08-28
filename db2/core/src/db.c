#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "db.h"

db_base *db_new(size_t a_cols, size_t a_lines) {
	db_base *res = malloc(sizeof(db_base));
	if(!res) {
		perror("db_new : arror alloc base");
		return NULL;
	}
	res->cols = array_new(a_cols);
	if(!res->cols) {
		fprintf(stderr, "db_new : error alloc types\n");
		return NULL;
	}

	res->lines = array_new(a_lines);
	if(!res->lines) {
		fprintf(stderr, "db_new : error alloc lines\n");
		return NULL;
	}

	return res;
}

int db_get_type(db_base *db, size_t c) {
	db_col *col = array_get(db->cols, c);
	if(!col) {
		fprintf(stderr, "db_get_type : error getting col\n");
		return -1;
	}
	return col->type;
}

void db_line_col_free(db_base *db, array *line, size_t c) {
	assert(db);
	assert(db->lines);
	assert(line);
	assert(c < db->cols->n);

	//fprintf(stderr, "%ld %ld\n", c, line->n);

	void *col = array_get(line, c);

	if(col) {
		int type = db_get_type(db, c);

		switch(type) {
			case INTEGER:
			case DOUBLE:
				free(col);
				break;
			case STRING:
				str_free(col);
				break;
			default:
				fprintf(stderr, "db_line_col_free : error getting column type(%d)\n", 
						type);
				free(col);
				break;
		}
		col = NULL;
	}
}

void db_line_free(db_base *db, array *line) {
	assert(db);

	if(line) {
		for(size_t c = 0; c < db->cols->n; c++) {
			db_line_col_free(db, line, c);
		}
		free(line);
	}
	line = NULL;
}

void db_free(db_base *db) {
	assert(db);

	for(size_t c = 0; c < db->cols->n; c++) {
		free(db->cols->data[c]);
	}

	for(size_t l = 0; l < db->lines->n; l++) {
		db_line_free(db, db->lines->data[l]);
	}
	free(db->cols);
	free(db->lines);
	free(db);
	db = NULL;
}

db_col *db_col_new(bool mandatory, int type, char *name, char *comment) {
	db_col *res = malloc(sizeof(db_col));
	if(!res) {
		perror("db_col_new : error alloc new col");
		return NULL;
	}
	res->mandatory = mandatory;
	res->type = type;
	strncpy(res->name, name, DB_COL_NAME_LEN);
	strncpy(res->comment, comment, DB_COL_COMMENT_LEN);

	return res;
}

db_base *db_col_add(db_base *db, bool mandatory, int type, char *name, char *comment) {
	array *line = NULL;
	db_col *col = db_col_new(mandatory, type, name, comment);
	if(!col) {
		fprintf(stderr, "db_col_add : error creating col\n");
		return NULL;
	}

	if(!array_append(db->cols, col)) {
		fprintf(stderr, "db_col_add : error adding column\n");
		return NULL;
	}

	for(size_t l = 0; l < db->lines->n; l++) {
		line = array_get(db->lines, l);
		if(!array_append(line, NULL)) {
			fprintf(stderr, "db_col_add : error adding NULL value for line %ld\n", l);
			return NULL;
		}
	}

	return db;
}

array *db_line_new(db_base *db) {
	assert(db);
	assert(db->cols);
	assert(db->cols->n > 0);

	array *res = array_new(db->cols->n);
	if(!res) {
		fprintf(stderr, "db_line_new : line creation failed");
		return NULL;
	}

	for(size_t c = 0; c < db->cols->n; c++) {
		array_append(res, NULL);
	}

	return res;
}

int db_line_get_int(db_base *db, array *line, size_t c) {
	assert(db_get_type(db, c) == INTEGER);

	return *((int *)array_get(line, c));
}

double db_line_get_double(db_base *db, array *line, size_t c) {
	assert(db_get_type(db, c) == DOUBLE);

	return *((double *)array_get(line, c));
}

char *db_line_get_string(db_base *db, array *line, size_t c) {
	assert(db_get_type(db, c) == STRING);

	return ((str *)array_get(line, c))->data;
}

void db_col_print_as_int(db_base *db, array *line, size_t c, FILE *file) {
	fprintf(file, "%d\t", db_line_get_int(db, line, c));
}

void db_col_print_as_double(db_base *db, array *line, size_t c, FILE *file) {
	fprintf(file, "%f\t", db_line_get_double(db, line, c));
}

void db_col_print_as_string(db_base *db, array *line, size_t c, FILE *file) {
	fprintf(file, "%s\t", db_line_get_string(db, line, c));
}

void db_col_print(db_base *db, array *line, size_t c, FILE *file) {
	int type = db_get_type(db, c);

	switch(type) {
		case INTEGER:
			db_col_print_as_int(db, line, c, file);
			break;

		case DOUBLE:
			db_col_print_as_double(db, line, c, file);
			break;

		case STRING:
			db_col_print_as_string(db, line, c, file);
			break;

		default:
			fprintf(stderr, "db_col_print : error getting column type(%d)\n",
					type);
			break;
	}
}

void db_line_print(db_base *db, array *line, FILE *file) {
	for(size_t c = 0; c < db->cols->n; c++) {
		db_col_print(db, line, c, file);
	}
	fprintf(file, "\n");
}

db_col *db_col_assert_type(db_base *db, size_t c, int type) {
	assert(c < db->cols->n);

	db_col *col = array_get(db->cols, c);
	if(!col) {
		fprintf(stderr, "db_col_assert_type : error getting col\n");
		return NULL;
	}

	if(col->type != type) {
		fprintf(stderr, "db_assert_type : db type(%d) != given type(%d)\n", col->type, type);
		return NULL;
	}
	return col;
}

/* free possible existing value and set new value by reference */
db_base* db_line_set(db_base *db, array *line, size_t c, int type, void *value, size_t alloc) {
	assert(db);
	assert(line);
	assert(c < db->cols->n);

	if(!db_col_assert_type(db, c, type)) {
		fprintf(stderr, "db_line_set : error on type\n");
		return NULL;
	}

	db_line_col_free(db, line, c);

	if(type == STRING) {
		if(value) {
			str *s = str_new(DB_STR_LEN_ALLOC);
			if(!s) {
				fprintf(stderr, "db_line_set : error creating str\n");
				return NULL;
			}
			str_set(s, value, alloc);
			array_set(line, c, s);
		}
		else {
			array_set(line, c, NULL);
		}
	}
	else {
		array_set(line, c, value);
	}

	return db;
}

db_base* db_line_set_null(db_base *db, array *line, size_t c, int type) {
	void *value = array_get(line, c);

	if(value) {
		if(type == STRING) {
			str_free(value);
		}
		else {
			free(value);
		}
		array_set(line, c, NULL);
	}

	return db;
}

db_base *db_insert(db_base *res, array *line) {
	assert(res);
	assert(line);

	if(!array_append(res->lines, line)) {
		fprintf(stderr, "db_insert : error adding element\n");
		return NULL;
	}

	return res;
}

void db_remove_nulls(db_base *db) {
	assert(db);

	array_remove_nulls(db->lines);
}

db_base *db_resize(db_base *db, double coef, double new_coef) {
	assert(db);
	assert(coef < new_coef);

	if(db->lines->n < db->lines->size * coef) {
		if(!array_resize(db->lines, db->lines->size * new_coef)) {
			fprintf(stderr, "db_resize : error realloc lines\n");
			return NULL;
		}
	}

	return db;
}

void db_delete(db_base *db, size_t l, bool keep_null) {
	assert(db);
	assert(l < db->lines->n);
	assert(db->lines->data[l]);

	db_line_free(db, db->lines->data[l]);

	if(!keep_null) {
		for(size_t i = l; i < db->lines->n; i++) {
			db->lines->data[i] = db->lines->data[i+1];
		}
		db->lines->n--;
	}
}

void db_write(db_base *db, FILE *file) {
	assert(db);
	assert(db->cols);
	assert(db->lines);

	db_col *col = NULL;

	fprintf(file, "cols:%ld lines:%ld\n", db->cols->n, db->lines->n);

	for(size_t c = 0; c < db->cols->n; c++) {
		col = array_get(db->cols, c);
		fprintf(file, "[%d][%d][%s][%s];", col->mandatory, col->type, col->name, col->comment);
	}

	fprintf(file, "\n");

	for(size_t l = 0; l < db->lines->n; l++) {
		db_line_write(db, file, l);
	}

	fflush(file);
}

void db_line_write(db_base *db, FILE *file, size_t l) {
	assert(db);
	assert(l < db->lines->n);
	assert(db->lines);
	assert(db->cols);

	void *value;
	array *line = db->lines->data[l];
	str *s;

	if(line) {
		for(size_t c = 0; c < db->cols->n; c++) {
			value = array_get(line, c);
			if(value) {
				int type = db_get_type(db, c);
				switch(type) {
					case INTEGER:
						fprintf(file, "%d", *(int *)value);
						break;
					case DOUBLE:
						fprintf(file, "%f", *(double *)value);
						break;
					case STRING:
						s = value;
						if(s->data) {
							fprintf(file, "%s", s->data);
						}
						break;
					default:
						fprintf(stderr, "db_line_write : error getting column type(%d)\n",
								type);
						break;
				}
			}
			fprintf(file, ";");
		}
		fprintf(file, "\n");
	}
}

/* alloc only if n_read > 0 */
char *db_file_readline(FILE *f, size_t alloc, size_t realloc_coef, size_t *n_read) {
	char *res;
	char c;
	size_t i;

	c = fgetc(f);

	if(c) {
		res = malloc(sizeof(char) * alloc);
		if(!res) {
			perror("db_file_readline : alloc failed");
			return NULL;
		}
	}

	for(i = 0; c != EOF && c !='\0'; i++) {
		/* realloc */
		if(i >= alloc) {
			alloc *= realloc_coef;
			res = realloc(res, sizeof(char) * alloc);
			if(!res) {
				perror("db_file_readline : realloc failed");
				return NULL;
			}
		}

		if(c == '\n') {
			break;
		}

		res[i] = c;
		c = fgetc(f);
	}
	res[i] = '\0';

	if(c == '\n') {
		i++;
	}

	*n_read = i;

	return res;
}

/*
 * Read columns from string and alloc values into db
 */
array *db_line_new_from_s(db_base *db, char *string, size_t alloc_s, size_t *n_cols) {
	assert(db);
	assert(string);
	assert(db->cols->n >= 1);
	assert(strlen(string) >= db->cols->n);

	//fprintf(stderr, "db_line_new_from_s : [%s]\n", string);

	array *line;
	db_col *col;
	size_t l;
	char *column;
	char *prec = NULL;
	char *cur = &string[0];
	int *i;
	double *d;


	/* new line without values allocation */
	line = db_line_new(db);
	if(!line) {
		fprintf(stderr, "db_line_new_from_s : error creating line\n");
		return NULL;
	}

	for(*n_cols = 0, l = 0; *n_cols < db->cols->n && cur; cur++, prec++) {

		/* new column */
		if(l == 0) {
			column = malloc(sizeof(char) * alloc_s);
			if(!column) {
				perror("db_line_new_from_s : error alloc string");
				return NULL;
			}
		}

		/* realloc */
		if(l >= alloc_s - 1) {
			alloc_s *= 2;
			column = realloc(column, alloc_s);
			if(!column) {
				perror("db_line_new_from_s : error realloc string");
				return NULL;
			}
		}

		if(cur - string > 0) {
			prec = cur - 1;
		}
		else {
			prec = NULL;
		}


		// end of current column
		if(*cur == ';' && (*prec != '\\' || !prec)) {
			column[l] = '\0';

			//fprintf(stderr, "db_line_new_from_s : column[%s]\n", column);

			col = array_get(db->cols, *n_cols);
			if(!col) {
				fprintf(stderr, "db_line_new_from_s : error getting col\n");
				return NULL;
			}

			// A string has been read
			if(l > 0) {
				/* Converting string to corresponding type and add to base */
				switch(col->type) {
					case INTEGER:
						i = malloc(sizeof(int));
						sscanf(column, "%d", i);
						db_line_set(db, line, *n_cols, INTEGER, i, 0);
						free(column);
						break;

					case DOUBLE:
						d = malloc(sizeof(double));
						sscanf(column, "%lf", d);
						db_line_set(db, line, *n_cols, DOUBLE, d, 0);
						free(column);
						break;

					case STRING:
						db_line_set(db, line, *n_cols, STRING, column, alloc_s);
						break;
				}
			}
			else {
				db_line_set_null(db, line, *n_cols, col->type);
			}

			(*n_cols)++;
			l = 0;
		}
		else {
			column[l++] = *cur;
		}
	}

	return line;
}


int db_read_bracket(char *dest, char *p, int *pos) {
	size_t n_read = 0;

	dest[0] = '\0';
	if(*p != '[') {
		fprintf(stderr, "read_bracket : error reading bracket. Missing [\n");
		return -1;
	}
	p++;

	while(p && *p != ']') {
		dest[n_read++] = *p++;
	}
	dest[n_read] = '\0';

	if(*p != ']') {
		fprintf(stderr, "read_bracket : error reading bracket. Missing ]\n");
		return -1;
	}
	p++;

	(*pos) += n_read + 2;

	return n_read;
}


db_base *db_col_adds(db_base *db, char *src) {
	char *p = src;
	char tmp[64];
	int mandatory;
	int type;
	char tmp_mandatory[2];
	char tmp_type[2];
	char name[DB_COL_NAME_LEN];
	char comment[DB_COL_COMMENT_LEN];

	for(int i = 0; p && *p; i++, p++) {
		tmp[i] = *p;

		if(tmp[i] == ';') {
			int pos = 0;
			if(db_read_bracket(&tmp_mandatory[0], &tmp[pos], &pos) < 1) {
				fprintf(stderr, "db_col_adds : error reading column mandatory : %s\n",
						&tmp_mandatory[0]);
				return NULL;
			}
			if(db_read_bracket(&tmp_type[0], &tmp[pos], &pos) < 1) {
				fprintf(stderr, "db_col_adds : error reading column type : %s\n",
						&tmp_type[0]);
				return NULL;
			}
			if(db_read_bracket(&name[0], &tmp[pos], &pos) < 1) {
				fprintf(stderr, "db_col_adds : error reading column name : %s\n",
						&name[0]);
				return NULL;
			}
			if(db_read_bracket(&comment[0], &tmp[pos], &pos) < 0) {
				fprintf(stderr, "db_col_adds : error reading column comment : %s\n",
						&comment[0]);
				return NULL;
			}

			mandatory = atoi(tmp_mandatory);
			type = atoi(tmp_type);

			if(!db_col_add(db, mandatory, type, name, comment)) {
				fprintf(stderr, "db_col_adds : error adding column\n");
				return NULL;
			}

			i = 0;
		}
	}
	return db;
}

db_base *db_file_load(db_base *db, FILE *file, size_t alloc_s) {
	assert(db);
	assert(db->lines->n == 0);
	assert(db->cols->n == 0);
	assert(db->cols);
	assert(db->lines);

	char *tmp;
	size_t n_read, n_cols, n_lines, n;
	array *line;

	// read first line (number of columns, lines)
	tmp = db_file_readline(file, 32, 2, &n_read);
	if(!tmp) {
		fprintf(stderr, "db_file_load : error reading first line\n");
		return NULL;
	}
	if(sscanf(tmp, "cols:%lu lines:%lu", &n_cols, &n_lines) != 2) {
		fprintf(stderr, "db_load : bad table description\n");
		return NULL;
	}
	free(tmp);

	// read second line (types)
	tmp = db_file_readline(file, 32, 2, &n_read);
	if(!tmp) {
		fprintf(stderr, "db_file_load : error reading second line\n");
		return NULL;
	}
	if(!db_col_adds(db, tmp)) {
		fprintf(stderr, "db_load : error adding columns\n");
		return NULL;
	}

	if(db->cols->n != n_cols) {
		fprintf(stderr, "db_load : columns [%ld]!=[%ld]\n", db->cols->n, n_cols);
		return NULL;
	}

	// Read every data lines and insert to base
	while(1) {
		tmp = db_file_readline(file, 256, 4, &n_read);
		if(!tmp) {
			fprintf(stderr, "db_file_load : error reading file line\n");
			return NULL;
		}

		if(n_read == 0) {
			break;
		}

		line = db_line_new_from_s(db, tmp, alloc_s, &n);
		if(!line) {
			fprintf(stderr, "db_file_load : error creating new line\n");
			return NULL;
		}

		if(n != n_cols) {
			fprintf(stderr, "db_file_load : line columns[%ld]!=[%ld]\n", n, n_cols);
			return NULL;
		}

		if(!db_insert(db, line)) {
			fprintf(stderr, "db_file_load : error inserting line\n");
			return NULL;
		}

		free(tmp);
	}


	return db;
}

void db_escape_string(char *string) {
	char *p = string;

	while(p) {

	}

}

