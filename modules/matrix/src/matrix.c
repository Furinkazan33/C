#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "matrix.h"

matrix *matrix_new(size_t lines, size_t cols, double init_value) {
	matrix *res = malloc(sizeof(matrix));
	if(!res) {
		perror("");
		return NULL;
	}
	res->lines = lines;
	res->cols = cols;
	res->values = malloc(sizeof(double) * lines * cols);
	if(!res->values) {
		perror("");
		return NULL;
	}

	for(size_t l = 0; l < lines; l++) {
		for(size_t c = 0; c < cols; c++) {
			res->values[l*cols + c] = init_value;
		}
	}

	return res;
}

matrix *matrix_new_id(size_t n) {
	matrix *res = matrix_new(n, n, 0);
	if(!res) {
		perror("");
		return NULL;
	}

	for(size_t i = 0; i < n; i++) {
		matrix_set_value(res, i, i, 1);
	}

	return res;
}

void matrix_free(matrix *m) {
	assert(m);

	free(m->values);
	free(m);
}

matrix *matrix_copy(matrix *m) {
	assert(m);
	matrix *res = matrix_new(m->lines, m->cols, 0);
	if(!res) {
		perror("");
		return NULL;
	}
	memcpy(res->values, m->values, sizeof(double) * m->lines * m->cols);

	return res;
}

void matrix_print(matrix *m) {
	for(size_t l = 0; l < m->lines; l++) {
		for(size_t c = 0; c < m->cols; c++) {
			printf("%f ", m->values[l*m->cols + c]);
		}
		printf("\n");
	}
}

matrix *matrix_set_value(matrix *m, size_t l, size_t c, double v) {
	assert(m);
	assert(l < m->lines);
	assert(c < m->cols);

	m->values[l*m->cols + c] = v;

	return m;
}

matrix *matrix_set_values(matrix *m, double *values) {
	for(size_t l = 0; l < m->lines; l++) {
		for(size_t c = 0; c < m->cols; c++) {
			matrix_set_value(m, l, c, values[l * m->cols + c]);
		 }
	}
	return m;
}

matrix *matrix_set_all(matrix *m, double value) {
	assert(m);

	for(size_t l = 0; l < m->lines; l++) {
		for(size_t c = 0; c < m->cols; c++) {
			matrix_set_value(m, l, c, value);
		}
	}
	return m;
}

double matrix_get_value(matrix *m, size_t l, size_t c) {
	assert(m);
	assert(l < m->lines);
	assert(c < m->cols);

	return m->values[l * m->cols + c];
}

double *matrix_get_value_pointer(matrix *m, size_t l, size_t c) {
	assert(m);
	assert(l < m->lines);
	assert(c < m->cols);

	return (m->values) + l * m->cols + c;
}

matrix *matrix_add(matrix *m1, matrix *m2) {
	assert(m1 && m1->values);
	assert(m2 && m2->values);
	assert(m1->lines == m2->lines);
	assert(m1->cols == m2->cols);

	matrix *res = matrix_copy(m1);
	if(!res) {
		perror("");
		return NULL;
	}

	for(size_t l = 0; l < res->lines; l++) {
		for(size_t c = 0; c < res->cols; c++) {
			matrix_set_value(res, l, c, matrix_get_value(res, l, c) + matrix_get_value(m2, l, c));
		}
	}

	return res;
}

matrix *matrix_sub(matrix *m1, matrix *m2) {
	assert(m1 && m1->values);
	assert(m2 && m2->values);
	assert(m1->lines == m2->lines);
	assert(m1->cols == m2->cols);

	matrix *res = matrix_copy(m1);
	if(!res) {
		perror("");
		return NULL;
	}

	for(size_t l = 0; l < res->lines; l++) {
		for(size_t c = 0; c < res->cols; c++) {
			matrix_set_value(res, l, c, matrix_get_value(res, l, c) - matrix_get_value(m2, l, c));
		}
	}

	return res;
}

/* mult 1 line with 1 col */
double _matrix_mult_lc(matrix *m1, size_t l, matrix *m2, size_t c) {
	assert(m1);
   	assert(m2);
	assert(m1->cols == m2->lines);

	double res = 0;

	for(size_t i = 0; i < m1->cols; i++) {
		res += *(matrix_get_value_pointer(m1, l, i)) * *(matrix_get_value_pointer(m2, i, c));
	}

	return res;
}

matrix *matrix_mult(matrix *m1, matrix *m2) {
	assert(m1);
	assert(m2);
	assert(m1->cols == m2->lines);

	matrix *res = matrix_new(m1->lines, m2->cols, 0);
	if(!res) {
		perror("");
		return NULL;
	}

	for(size_t l = 0; l < m1->lines; l++) {
		for(size_t c = 0; c < m2->cols; c++) {
			matrix_set_value(res, l, c, _matrix_mult_lc(m1, l, m2, c));
		}
	}

	return res;
}

matrix *matrix_mult_scal(matrix *m, double scal) {
	assert(m);

	matrix *res = matrix_copy(m);
	if(!res) {
		perror("");
		return NULL;
	}

	for(size_t l = 0; l < res->lines; l++) {
		for(size_t c = 0; c < res->cols; c++) {
			matrix_set_value(res, l, c, matrix_get_value(m, l, c) * scal);
		}
	}
	return res;
}

int matrix_equal(matrix *m1, matrix *m2, int (*double_equal)(double, double)) {
	assert(m1);
	assert(m2);
	assert(m1->lines == m2->lines);
	assert(m1->cols == m2->cols);

	for(size_t l = 0; l < m1->lines; l++) {
		for(size_t c = 0; c < m1->cols; c++) {
			if(!double_equal(matrix_get_value(m1, l, c), matrix_get_value(m2, l, c))) {
				return 0;
			}
		}
	}
	return 1;
}
