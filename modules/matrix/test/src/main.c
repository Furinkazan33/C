#include <stdio.h>
#include <fcntl.h>
#include "matrix.h"

int double_equal(double a, double b) {
	return a == b;
}


int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s", argv[0]);
		exit(1);
	}

	double v1[3][3] = { { 0, 1, 2 },
						{ 3, 4, 5 },
						{ 6, 7, 8 } };
	double v2[3][2] = { { 0, 1 },
						{ 2, 3 },
						{ 4, 5 } };

	matrix *m1 = matrix_new(3, 3, 0);
	matrix *m2 = matrix_new(3, 2, 0);

	matrix_set_values(m1, &v1[0][0]);
	matrix_set_values(m2, &v2[0][0]);

	matrix *m3 = matrix_mult(m1, m2);

	matrix_print(m1); puts("");
	matrix_print(m2); puts("");
	matrix_print(m3); puts("");

	matrix *id = matrix_new_id(3);
	matrix_print(id); puts("");
	matrix *m4 = matrix_mult(m1, id);
	matrix_print(m4); puts("");

	if(matrix_equal(m1, m4, double_equal)) {
		printf("m1 and m4 identical\n");
	}


	matrix_free(m1);
	matrix_free(m2);
	matrix_free(m3);
	matrix_free(id);
	matrix_free(m4);

	return 0;
}
