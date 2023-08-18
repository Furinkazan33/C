#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "db.h"

db_base *db_new(size_t a_cols, size_t a_lines) {
	db_base *res = malloc(sizeof(*res));
	if(!res) {
		perror("");
		return NULL;
	}
	res->types = malloc(sizeof(int) * a_cols);
	if(!res->types) {
		perror("");
		return NULL;
	}

	res->lines = malloc(sizeof(void **) * a_lines);
	if(!res->lines) {
		perror("");
		return NULL;
	}

	res->a_cols = a_cols;
	res->a_lines = a_lines;

	res->n_cols = 0;
	res->n_lines = 0;

	return res;
}

db_base *db_add_col(db_base *res, int type) {
	int *tmp;
	void **line;

	/* realloc */
	if(res->n_cols >= res->a_cols) {
		tmp = realloc(res->types, res->a_cols * 2);
		if(!tmp) {
			perror("");
			return NULL;
		}
		res->types = tmp;

		for(size_t l = 0; l < res->n_lines; l++) {
			line = realloc(res->lines[l], res->a_cols * 2);
			if(!line) {
				perror("");
				return NULL;
			}
			res->lines[l] = line;
		}

		res->a_cols *= 2;
	}

	res->types[res->n_cols] = type;

	/* Set new col's value to NULL on each line */
	for(size_t i = 0; i < res->n_lines; i++) {
		res->lines[i][res->n_cols] = NULL;
	}

	res->n_cols++;

	return res;
}

void **db_line_new(db_base *db, bool alloc_values, size_t alloc_for_strings) {
	assert(db);

	void **res = malloc(sizeof(void *) * db->n_cols);
	if(!res) {
		perror("db_line_new : line alloc failed");
		return NULL;
	}

	db_string *s;

	for(size_t c = 0; c < db->n_cols; c++) {
		/* alloc for db_string */
		if(db->types[c] == STRING) {
			res[c] = malloc(sizeof(db_string));
			if(!res[c]) {
				perror("db_line_new : db_string alloc failed");
				return NULL;
			}
		}

		if(alloc_values) {
			switch(db->types[c]) {
				case INTEGER:
					res[c] = malloc(sizeof(int));
					if(!res[c]) {
						perror("db_line_new : value alloc failed");
						return NULL;
					}
					break;

				case DOUBLE:
					res[c] = malloc(sizeof(double));
					if(!res[c]) {
						perror("db_line_new : value alloc failed");
						return NULL;
					}
					break;

				case STRING:
					s = res[c];
					s->value = malloc(sizeof(char) * (alloc_for_strings + 1));
					if(!s->value) {
						perror("");
						return NULL;
					}
					s->alloc = alloc_for_strings;
					break;

				default:
					fprintf(stderr, "db_line_new : unknown type : %d\n", db->types[c]);
					return NULL;
			}
		}
	}

	return res;
}

void db_line_set_int(db_base *db, void **line, size_t c, const int value) {
	assert(db);
	assert(line);
	assert(line[c]);
	assert(c < db->n_cols);
	assert(db->types[c] == INTEGER);

	*(int *)line[c] = value;
}

void db_line_set_double(db_base *db, void **line, size_t c, const double value) {
	assert(db);
	assert(line);
	assert(line[c]);
	assert(c < db->n_cols);
	assert(db->types[c] == DOUBLE);

	*(double *)line[c] = value;
}

db_base *db_line_set_string(db_base *db, void **line, size_t c, const char *value) {
	assert(db);
	assert(line);
	assert(line[c]);
	assert(c < db->n_cols);
	assert(db->types[c] == STRING);

	db_string *ds = line[c];

	/* realloc string */
	while(strlen(value) > ds->alloc) {
		ds->value = realloc(ds->value, ds->alloc * 2);
		if(!ds->value) {
			perror("db_line_set_string : string realloc failed");
			return NULL;
		}
		ds->alloc *= 2;
	}

	strcpy(ds->value, value);

	return db;
}

db_base* db_line_set_byref(db_base *db, void **line, size_t c, int type, void *value, size_t alloc) {
	assert(db);
	assert(line);
	assert(c < db->n_cols);
	assert(type == db->types[c]);

	if(type == STRING) {
		db_string *ds = line[c];
		assert(ds->value == NULL); // line was created without allocation - db_line_new(db, 0)
		ds->value = value;
		ds->alloc = alloc;
	}
	else {
		assert(line[c] == NULL); // line was created without allocation - db_line_new(db, 0)
		line[c] = value;
	}

	return db;
}

db_base *db_insert(db_base *res, void **line, size_t n) {
	assert(res);
	assert(line);
	assert(n == res->n_cols);

	/* realloc */
	void ***tmp;
	if(res->n_lines >= res->a_lines) {
		tmp = realloc(res->lines, sizeof(void **) * res->a_lines * 2);
		if(!tmp) {
			perror("");
			return NULL;
		}
		res->lines = tmp;
		res->a_lines *= 2;
	}

	res->lines[res->n_lines++] = line;

	return res;
}

void db_remove_nulls(db_base *db) {
	assert(db);

	for(size_t i = 0; i < db->n_lines; ) {
		if(db->lines[i] == NULL) {
			for(size_t j = i; j < db->n_lines - 1; j++) {
				db->lines[j] = db->lines[j+1];
			}
			db->n_lines--;
		}
		if(db->lines[i] != NULL) {
			i++;
		}
	}
}

db_base *db_resize(db_base *db, double coef, double new_coef) {
	assert(db);
	assert(coef < new_coef);

	if(db->n_lines < db->a_lines * coef) {
		void ***tmp = realloc(db->lines, sizeof(void **) * db->a_lines * new_coef);
		if(!tmp) {
			perror("");
			return NULL;
		}
		db->lines = tmp;
		db->a_lines *= new_coef;
	}

	return db;
}

void db_delete(db_base *db, size_t l, bool keep_null) {
	assert(db);
	assert(l < db->n_lines);
	assert(db->lines[l]);

	db_free_line(db, l);

	if(!keep_null) {
		for(size_t i = l; i < db->n_lines; i++) {
			db->lines[i] = db->lines[i+1];
		}
		db->n_lines--;
	}
}

void db_free_line(db_base *db, size_t l) {
	assert(db);
	assert(l < db->n_lines);
	assert(db->lines[l]);

	for(size_t c = 0; c < db->n_cols; c++) {
		if(db->lines[l][c]) {
			if(db->types[c] == STRING) {
				free(((db_string *)db->lines[l][c])->value);
			}
			free(db->lines[l][c]);
		}
	}
	free(db->lines[l]);
	db->lines[l] = NULL;
}

void db_free(db_base *db) {
	assert(db);

	free(db->types);

	for(size_t l = 0; l < db->n_lines; l++) {
		if(db->lines[l]) {
			db_free_line(db, l);
		}
	}
	free(db->lines);

	free(db);
}

void db_print(db_base *db) {
	printf("a_cols=%zu, n_cols=%zu, a_lines=%zu, n_lines=%zu\n",
			db->a_cols, db->n_cols, db->a_lines, db->n_lines);
	for(size_t l = 0; l < db->n_lines; l++) {
		db_line_print(db, l);
	}
}

void db_line_print(db_base *db, size_t l) {
	void *value;
	void **line = db->lines[l];
	db_string *s;

	if(line) {
		for(size_t c = 0; c < db->n_cols; c++) {
			value = line[c];
			if(value) {
				switch(db->types[c]) {
					case INTEGER:
						printf("%d\t", *(int *)value);
						break;
					case DOUBLE:
						printf("%f\t", *(double *)line[c]);
						break;
					case STRING:
						s = line[c];
						printf("%s\t", s->value);
						break;
					default:
						fprintf(stderr, "db_line_print : unknown type : %d", db->types[c]);
						break;
				}
			}
			else {
				printf("%p\t", NULL);
			}
		}
	}
	else {
		printf("%p", NULL);
	}
	printf("\n");
}

