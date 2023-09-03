#include <stdlib.h>

/*
 * matrix manipulation.
 *
 * types defined : matrix
 * prefix used for functions : matrix_.
 *
 */

typedef struct matrix {
	size_t lines;
	size_t cols;
	double *values;
} matrix;

matrix *matrix_new(size_t lines, size_t cols, double init_value);
matrix *matrix_new_id(size_t n);
void matrix_free(matrix *m);
void matrix_print(matrix *m);

/* helpers */
matrix *matrix_set_value(matrix *m, size_t l, size_t c, double value);
matrix *matrix_set_values(matrix *m, double *values);
matrix *matrix_set_all(matrix *m, double value);
double matrix_get_value(matrix *m, size_t l, size_t c);
double *matrix_get_value_pointer(matrix *m, size_t l, size_t c);

/* operations on matrices */
matrix *matrix_copy(matrix *m);
matrix *matrix_add(matrix *m1, matrix *m2);
matrix *matrix_sub(matrix *m1, matrix *m2);
matrix *matrix_mult(matrix *m1, matrix *m2);
matrix *matrix_mult_scal(matrix *m, double scal);
/* decide wether 2 matrices are identical based on the given double equality function given */
int matrix_equal(matrix *m1, matrix *m2, int (*double_equal)(double, double));

