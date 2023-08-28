#include <stdlib.h>
#include <stdbool.h>
#include "array.h"
#include "str.h"

/*
 * Generic database containing any db_type of data.
 *
 * types defined : db_base, db_type
 * prefix used for functions : db_
 *
 */

#define DB_COL_NAME_LEN 63
#define DB_COL_COMMENT_LEN 255
#define DB_STR_LEN_ALLOC 63

/* Available types.
 * Adding a type suggests create/modify following functions :
 * - db_line_new
 * - db_line_write
 */
typedef enum db_type { INTEGER=0, DOUBLE, STRING } db_type;

/* Column definition */
typedef struct db_col {
	char name[DB_COL_NAME_LEN];
	char comment[DB_COL_COMMENT_LEN];
	int type;
	bool mandatory;
} db_col;

/*
 * Main database structure
 * types and lines are reallocated if needed when adding.
 */
typedef struct db_base {
	array *cols;	// array of db_col
	array *lines;	// array of array of void *
} db_base;


/*
 * memory allocations
 */

/* allocating base */
db_base *db_new(size_t a_cols, size_t a_lines);
db_col *db_col_new(bool mandatory, int type, char *name, char *comment);
db_base *db_col_add(db_base *db, bool mandatory, int type, char *name, char *comment);
/* alloc and return new line. Alloc every columns or NULL */
array *db_line_new(db_base *db);

/* check fields before insert */
db_base *db_line_check_fields(db_base *db, array *line); //TODO

/*
 * No memory allocation
 */

/* set value by reference. In case of strings, set string->alloc_len with len */
db_base *db_line_set(db_base *db, array *line, size_t c, int t, void *val, size_t len);

/* append line containing n columns */
db_base *db_insert(db_base *res, array *line);
/* remove every NULL from lines */
void db_remove_nulls(db_base *db);

/* resize to a_lines * new_coef if n_lines < a_lines * coef */
db_base *db_resize(db_base *db, double coef, double new_coef);

/* free line from base. if keep_null, base->lines[l] = NULL else base->n_lines-- */
void db_delete(db_base *db, size_t l, bool keep_null);



/*
 * free functions
 */

void db_line_col_free(db_base *db, array *line, size_t c);
void db_line_free(db_base *db, array *line);
void db_free(db_base *db);


/*
 * Printing
 */

void db_write(db_base *db, FILE *file);
void db_line_write(db_base *db, FILE *file, size_t l);


/* load file into db (alloc_s is char * size allocated for reading 1 column per line) */
db_base *db_file_load(db_base *db, FILE *file, size_t alloc_s);




