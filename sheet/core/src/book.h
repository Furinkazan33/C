#include <stdlib.h>
#include "array.h"

#define MAX_NAME_LEN 32
#define MAX_PATH_LEN 256

#define MAX_SHEETS 16
#define MAX_COLS  4096
#define MAX_CELLS 4096
#define MAX_VALUE_LEN 128

typedef struct book {
	char name[MAX_NAME_LEN];
	char path[MAX_PATH_LEN];
	array *sheets;
} book;

typedef struct sheet {
	char name[MAX_NAME_LEN];
	array *cols;
} sheet;

typedef struct col {
	unsigned int n;
	array *cells;
} col;

typedef struct cell {
	unsigned int n;
	size_t size;
	char *value;
} cell;

typedef struct range {
	array *cols;
} range;


/*
 * Alloc & free
 */
book *book_new(char *path, char *name, size_t init_size);
sheet *sheet_new(char *name, size_t init_size);
col *col_new(size_t init_size, size_t n);
cell *cell_new(size_t init_size, size_t n);
range *range_new(size_t init_size);

void book_free(void *bo);
void sheet_free(void *sh);
void col_free(void *co);
void cell_free(void *ce);
void range_free(void *ra);


cell *cell_set_value(cell *ce, char *value, size_t len);


