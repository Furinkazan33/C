#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "db.h"


/*
 * memory allocations
 */
db_base *db_new(size_t a_cols, size_t a_lines, char *name, char *comment) {
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

	strncpy(res->name, name, DB_MAX_NAME_LEN);
	strncpy(res->comment, comment, DB_MAX_COMMENT_LEN);

	res->current_id = 0;

	return res;
}

db_base *db_new_with_id(size_t a_cols, size_t a_lines, char *name, char *comment) {
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

	strncpy(res->name, name, DB_MAX_NAME_LEN);
	strncpy(res->comment, comment, DB_MAX_COMMENT_LEN);

	res->current_id = 0;


	res = db_col_add(res, true, true, INTEGER, "id", "built-in unique id");
	if(!res) {
		fprintf(stderr, "db_new : call to db_col_add returned NULL\n");
		return NULL;
	}

	return res;
}

db_col *db_col_new(bool mandatory, bool unique, int type, char *name, char *comment) {
	assert(name);

	db_col *res = malloc(sizeof(db_col));
	if(!res) {
		perror("db_col_new : error alloc new col");
		return NULL;
	}
	res->mandatory = mandatory;
	res->unique = unique;
	res->type = type;
	strncpy(res->name, name, DB_COL_MAX_NAME_LEN);
	strncpy(res->comment, comment, DB_COL_MAX_COMMENT_LEN);

	return res;
}

array *db_line_new(db_base *db) {
	if(!db) {
		return NULL;
	}
	assert(db->cols->n > 0);

	array *res = array_new(db->cols->size);
	if(!res) {
		fprintf(stderr, "db_line_new : line creation failed");
		return NULL;
	}
#ifdef DEBUG1
	fprintf(stdout, "db_line_new : appending %ld NULL values to new line\n", 
			db->cols->n);
	fflush(stdout);
#endif

	for(size_t c = 0; c < db->cols->n; c++) {
		array_append(res, NULL);
	}

	return res;
}

array *db_line_new_with_id(db_base *db) {
	if(!db) {
		return NULL;
	}
	assert(db->cols->n > 0);

	array *res = array_new(db->cols->size);
	if(!res) {
		fprintf(stderr, "db_line_new : line creation failed");
		return NULL;
	}
#ifdef DEBUG1
	fprintf(stdout, "db_line_new : appending %ld NULL values to new line\n", 
			db->cols->n);
	fflush(stdout);
#endif

	int *id = malloc(sizeof(int));
	if(!id) {
		fprintf(stderr, "db_line_new : call to malloc returned NULL\n"); 
		return NULL;
	}
	*id = db->current_id++;
	array_append(res, id);

	for(size_t c = 1; c < db->cols->n; c++) {
		array_append(res, NULL);
	}

	return res;
}

/*
 * Accessors
 */

int db_get_type(db_base *db, size_t c) {
	assert(db);
	assert(c < db->cols->n);

	db_col *col = db->cols->data[c];
	if(!col) {
		fprintf(stderr, "db_get_type : error getting col\n");
		return -1;
	}
	return col->type;
}


array *db_find(db_base *db, array *search, int (*cmp)(void *, void *)) {
	assert(db);
	assert(search);
	assert(cmp || db->lines->cmp);

	if(cmp) {
		db_set_cmp(db, cmp);
	}

	return array_find(db->lines, search);
}

void db_set_cmp(db_base *db, int (*cmp)(void *, void *)) {
	assert(db);
	db->lines->cmp = cmp;
}

int db_cmp_id(void *id1, void *id2) {
	return *(int *)id1 - *(int *)id2;
}

array *db_find_by_id(db_base *db, int id) {
	array *search = db_line_new(db);
	if(!search) {
		fprintf(stderr, "db_find_by_id : call to db_line_new returned NULL\n");
		return NULL;
	}

	int *i = malloc(sizeof(int));
	if(!i) {
		fprintf(stderr, "db_find_by_id : call to malloc returned NULL\n");
		return NULL;
	}
	*i = id;
	if(!db_line_set(db, search, 0, INTEGER, i, 0)) {
		fprintf(stderr, "db_find_by_id : call to db_line_set returned NULL\n");
		return NULL;
	}

	array *res = db_find(db, search, db_cmp_id);

	db_line_free(db, search);

	return res;
}

array *db_find_all(db_base *db, array *search, int (*cmp)(void *, void *), size_t init_alloc) {
	assert(db);
	assert(search);
	assert(cmp || db->lines->cmp);

	if(cmp) {
		db_set_cmp(db, cmp);
	}

	return array_find_all(db->lines, search, init_alloc);
}

/*
 * Modifiers
 */

db_base *db_col_add(db_base *db, bool mandatory, bool unique, int type, char *name, char *comment) {
	if(!db) {
		return NULL;
	}
	array *line = NULL;
	db_col *col = db_col_new(mandatory, unique, type, name, comment);
	if(!col) {
		fprintf(stderr, "db_col_add : error creating col\n");
		return NULL;
	}

#ifdef DEBUG1
	fprintf(stdout, "db_col_add : appending col [%s] to db\n", name);
	fflush(stdout);
#endif
	if(!array_append(db->cols, col)) {
		fprintf(stderr, "db_col_add : error adding column\n");
		return NULL;
	}

#ifdef DEBUG1
	fprintf(stdout, "db_col_add : appending NULL to every existing %ld lines\n", 
			db->lines->n);
	fflush(stdout);
#endif
	for(size_t l = 0; l < db->lines->n; l++) {
		line = db->lines->data[l];
		if(!array_append(line, NULL)) {
			fprintf(stderr, "db_col_add : error adding NULL value for line %ld\n", l);
			return NULL;
		}
	}

	return db;
}

//TODO check required fields
db_base *db_insert(db_base *res, array *line) {
	if(!res) {
		fprintf(stderr, "db_insert : NULL parameter\n");
		return NULL;
	}
	assert(line);

#ifdef DEBUG1
	fprintf(stdout, "db_insert : appending line to db\n");
	fflush(stdout);
#endif
	if(!array_append(res->lines, line)) {
		fprintf(stderr, "db_insert : error adding element\n");
		return NULL;
	}

	return res;
}

db_col *db_col_assert_type(db_base *db, size_t c, int type) {
	assert(db);
	assert(c < db->cols->n);

	db_col *col = db->cols->data[c];
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
db_base *db_line_set(db_base *db, array *line, size_t c, int type, void *value, size_t alloc_len) {
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
			str *s = str_new(DB_STR_INIT_LEN);
			if(!s) {
				fprintf(stderr, "db_line_set : error creating str\n");
				return NULL;
			}
			str_set(s, value, alloc_len);
			array_replace(line, c, s);
		}
		else {
			array_replace(line, c, NULL);
		}
	}
	else {
		array_replace(line, c, value);
	}

	return db;
}

/* TODO: check not required field
 * set NULL value in line's column */
db_base *db_line_set_null(db_base *db, array *line, size_t c, int type) {
	assert(db);
	assert(line);
	assert(c < db->cols->n);
	assert(db_get_type(db, c) == type);
	
	void *value = line->data[c];

	if(value) {
		if(type == STRING) {
			str_free(value);
		}
		else {
			free(value);
		}
		array_replace(line, c, NULL);
	}

	return db;
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



/*
 * free functions
 */

void db_line_col_free(db_base *db, array *line, size_t c) {
	assert(db);
	assert(db->lines);
	assert(line);
	assert(c < db->cols->n);

	void *col = line->data[c];

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



/*
 * Printing
 */

void db_write(db_base *db, FILE *file) {
	assert(db);
	assert(db->cols);
	assert(db->lines);

	db_col *col = NULL;

	fprintf(file, "cols:%ld lines:%ld\n", db->cols->n, db->lines->n);

	for(size_t c = 0; c < db->cols->n; c++) {
		col = db->cols->data[c];
		fprintf(file, "[%d][%d][%d][%s][%s];", col->mandatory, col->unique, col->type, col->name, col->comment);
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
			value = line->data[c];
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



/*
 * Files manipulation
 */

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
	}
	*n_read = i;

	return res;
}

int db_strchr(char *s, char c) {
	assert(s);
#ifdef DEBUG3
	fprintf(stdout, "db_strchr : [%s][%c]\n", s, c);
#endif

	for(int i = 0; s[i]; i++) {
		if(s[i] == c) {
			if(i == 0 || (i > 0 && s[i-1] != '\\')) {
#ifdef DEBUG3
				fprintf(stdout, "db_strchr : result : [%d]\n", i);
#endif
				return i;
			}
		}
	}
#ifdef DEBUG3
	fprintf(stdout, "db_strchr : result : [-1]\n");
#endif

	return -1;
}

char *db_read_col_as_string(char *src, size_t len) {
#ifdef DEBUG2
	fprintf(stdout, "db_read_col_as_string : [%s][%ld]\n", src, len);
#endif
	assert(src);

	char *res = NULL;

	if(len > 0) {
		res = malloc(sizeof(char) * (len + 1));
		if(!res) {
			perror("db_read_col_as_string : call to malloc returned NULL");
			return NULL;
		}
		strncpy(res, src, len);
		res[len] = '\0';
	}
#ifdef DEBUG2
	fprintf(stdout, "db_read_col_as_string : result : [%s]\n", res);
#endif

	return res;
}

int *db_read_col_as_int(char *src, size_t len) {
#ifdef DEBUG2
	fprintf(stdout, "db_read_col_as_int : [%s][%ld]\n", src, len);
#endif
	assert(src);
	assert(len < DB_FILE_READ_INT_LEN);

	char tmp[DB_FILE_READ_INT_LEN];
	int *res = NULL;

	if(len > 0) {
		strncpy(tmp, src, len);
		tmp[len] = '\0';

		res = malloc(sizeof(int));
		if(!res) {
			perror("db_read_col_as_int : call to malloc returned NULL");
			return NULL;
		}
		*res = atoi(tmp);
#ifdef DEBUG2
		fprintf(stdout, "db_read_col_as_int : result : [%d]\n", *res);
#endif
	}
	else {
#ifdef DEBUG2
		fprintf(stdout, "db_read_col_as_int : result : [null]\n");
#endif
	}
	return res;
}

double *db_read_col_as_double(char *src, size_t len) {
#ifdef DEBUG2
	fprintf(stdout, "db_read_col_as_double : [%s][%ld]\n", src, len);
#endif
	assert(src);
	assert(len < DB_FILE_READ_DOUBLE_LEN);

	double *res = NULL;
	char tmp[DB_FILE_READ_DOUBLE_LEN];

	if(len > 0) {
		strncpy(tmp, src, len);
		tmp[len] = '\0';

		res = malloc(sizeof(double));
		if(!res) {
			perror("db_read_col_as_double : call to malloc returned NULL");
			return NULL;
		}
		*res = atof(tmp);
#ifdef DEBUG2
		fprintf(stdout, "db_read_col_as_double : result : [%f]\n", *res);
#endif
	}
	else {
#ifdef DEBUG2
		fprintf(stdout, "db_read_col_as_double : result : [null]\n");
#endif
	}
	return res;
}

/* Read columns from string and alloc values into db */
array *db_line_new_from_s(db_base *db, char *string) {
	assert(db);
	assert(string);
	assert(db->cols->n >= 1);
	assert(strlen(string) >= db->cols->n);
#ifdef DEBUG1
	fprintf(stdout, "db_line_new_from_s : [%s]\n", string);
#endif

	db_col *col = NULL;
	void *value = NULL;
	char *p = string;
	int len;

	/* new line without values allocation */
	array *line = db_line_new(db);
	if(!line) {
		fprintf(stderr, "db_line_new_from_s : error creating line\n");
		return NULL;
	}

	for(size_t c = 0; c < db->cols->n; c++, p += len + 1) {
		col = db->cols->data[c];
		if(!col) {
			fprintf(stderr, "db_line_new_from_s : error getting col\n");
			return NULL;
		}

		// get next separator not escaped
		len = db_strchr(p, ';');
		if(len < 0) {
			fprintf(stderr, "db_line_new_from_s : error reading value\n");
			return NULL;
		}

		switch(col->type) {
			case INTEGER:
				value = db_read_col_as_int(p, len);
				break;

			case DOUBLE:
				value = db_read_col_as_double(p, len);
				break;

			case STRING:
				value = db_read_col_as_string(p, len);
				break;
		}
		if(value) {
			if(!db_line_set(db, line, c, col->type, value, len)) {
				fprintf(stderr, "db_line_new_from_s : call to db_line_set returned NULL\n");
				return NULL;
			}
		}
		else {
			if(!db_line_set_null(db, line, c, col->type)) {
				fprintf(stderr, "db_line_new_from_s : call to db_line_set_null returned NULL\n");
				return NULL;
			}
		}
	}

	return line;
}

int db_read_bracket(char *dest, char *p, int *pos) {
	size_t n_read = 0;
#ifdef DEBUG2
	fprintf(stdout, "db_read_bracket : reading from %s\n", p);
#endif

	dest[0] = '\0';
	if(*p != '[') {
		fprintf(stderr, "read_bracket : error reading bracket. Missing [\n");
		fprintf(stderr, "read_bracket : %s\n", p);
		return -1;
	}
	p++;

	while(p && *p != ']') {
		dest[n_read++] = *p++;
	}
	dest[n_read] = '\0';

	if(*p != ']') {
		fprintf(stderr, "read_bracket : error reading bracket. Missing ]\n");
		fprintf(stderr, "read_bracket : %s\n", p);
		return -1;
	}
	p++;

	(*pos) += n_read + 2;

#ifdef DEBUG2
	fprintf(stdout, "db_read_bracket : dest=%s\n", dest);
#endif

	return n_read;
}

db_base *db_col_adds(db_base *db, char *src) {
	char *p = src;
	char tmp[64];
	int mandatory;
	int unique;
	int type;
	char tmp_mandatory[2];
	char tmp_unique[2];
	char tmp_type[2];
	char name[DB_COL_MAX_NAME_LEN];
	char comment[DB_COL_MAX_COMMENT_LEN];

	for(int i = 0; p && *p; p++) {
		tmp[i] = *p;

		if(tmp[i] == ';') {
			tmp[i+1] = '\0';
			int pos = 0;
			if(db_read_bracket(&tmp_mandatory[0], &tmp[pos], &pos) < 1) {
				fprintf(stderr, "db_col_adds : error reading column mandatory : %s\n",
						&tmp_mandatory[0]);
				return NULL;
			}
			if(db_read_bracket(&tmp_unique[0], &tmp[pos], &pos) < 1) {
				fprintf(stderr, "db_col_adds : error reading column unique : %s\n",
						&tmp_unique[0]);
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
			unique = atoi(tmp_unique);
			type = atoi(tmp_type);

#ifdef DEBUG1
			fprintf(stdout, "db_col_adds : adding column %s\n", name);
#endif
			if(!db_col_add(db, mandatory, unique, type, name, comment)) {
				fprintf(stderr, "db_col_adds : error adding column\n");
				return NULL;
			}

			i = 0;
		}
		else {
			i++;
		}
	}
	return db;
}

db_base *db_file_load(db_base *db, FILE *file) {
	assert(db);
	assert(db->lines->n == 0);
	assert(db->cols->n == 0);
	assert(db->cols);
	assert(db->lines);

	char *tmp;
	size_t n_read, n_cols, n_lines;
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

		line = db_line_new_from_s(db, tmp);
		if(!line) {
			fprintf(stderr, "db_file_load : error creating new line\n");
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

