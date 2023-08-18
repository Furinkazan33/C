#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "book.h"

int main(int argc, char **argv) {

	if(argc != 1) {
		printf("usage : %s\n", argv[0]);
		return -1;
	}

	book *bo = book_new(argv[1], "Toto", 4);

	sheet *s1 = sheet_new("Sheet1", 16);

	array_append(bo->sheets, s1);

	col *co;
	cell *ce;
	/* addding cols to sheet */
	for(size_t i = 0; i < 2; i++) {
		co = col_new(10, i);

		/* adding cells to col */
		for(size_t j = 0; j < 10; j++) {
			ce = cell_new(64, j);
			ce = cell_set_value(ce, "toto", 4);
			array_append(co->cells, ce);
		}
		array_append(s1->cols, co);
	}

	printf("%s\n", ((cell *)co->cells->data[0])->value);


	book_free(bo);


	return 0;
}


