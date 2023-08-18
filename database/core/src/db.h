#include <stdlib.h>
#include <stdbool.h>

/*
 * Generic database containing any db_type of data.
 *
 * types defined : db_base, db_type, db_string
 * prefix used for functions : db_
 *
 */



/* Available types.
 * Adding a type suggests create/modify following functions :
 * - db_line_set_YOURTYPE
 * - db_line_new
 * - db_line_print
 */
typedef enum db_type { INTEGER=0, DOUBLE, STRING } db_type;

/*
 * Main database structure
 * types and lines are reallocated if needed when adding.
 */
typedef struct db_base {
	size_t a_cols;	// number of allocated columns
	size_t a_lines;	// number of allocated lines

	size_t n_cols;	// number of current columns
	size_t n_lines;	// number of current lines

	int *types;		// types of each column
	void ***lines;	// each line is a void **
} db_base;

/* structure used for STRING type */
typedef struct db_string {
	size_t alloc;
	char *value;
} db_string;


/*
 * memory allocations
 */

/* allocating base, base->types and base->lines with given parameters */
db_base *db_new(size_t a_cols, size_t a_lines);
/* add type to base->types */
db_base *db_add_col(db_base *res, int type);
/* alloc and return new line. Alloc every columns or NULL */
void **db_line_new(db_base *db, bool alloc_values, size_t alloc_for_strings);

/* set line's column value by copying given value */
void db_line_set_int(db_base *db, void **line, size_t c, const int value);
void db_line_set_double(db_base *db, void **line, size_t c, const double value);
db_base *db_line_set_string(db_base *db, void **line, size_t c, const char *value);


/*
 * No memory allocation
 */

/* set value by reference. In case of strings, set db_string->alloc */
db_base *db_line_set_byref(db_base *db, void **line, size_t c, int any_type, void *value, size_t alloc);
/* append line containing n columns */
db_base *db_insert(db_base *res, void **line, size_t n);
/* remove every NULL from lines */
void db_remove_nulls(db_base *db);

/* resize to a_lines * new_coef if n_lines < a_lines * coef */
db_base *db_resize(db_base *db, double coef, double new_coef);

/* free line from base. if keep_null, base->lines[l] = NULL else base->n_lines-- */
void db_delete(db_base *db, size_t l, bool keep_null);


/*
 * free functions
 */

void db_free_line(db_base *db, size_t l);
void db_free_lines(db_base *db);
void db_free(db_base *db);


/*
 * Printing
 */

void db_print(db_base *db);
void db_line_print(db_base *db, size_t l);



