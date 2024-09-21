#include <stdlib.h>
#include <stdbool.h>
#include "parray.h"
#include "str.h"

/*
 * Generic database containing any db_type of data.
 *
 * types defined : db_base, db_type
 * prefix used for functions : db_
 *
 */

#define DB_MAX_NAME_LEN 64			// maximum len
#define DB_MAX_COMMENT_LEN 512		// maximum len
#define DB_COL_MAX_NAME_LEN 32		// maximum len
#define DB_COL_MAX_COMMENT_LEN 256	// maximum len
#define DB_STR_INIT_LEN 63			// init. len alloc. for str
#define DB_FILE_READ_INT_LEN 16		// len for reading int values from file
#define DB_FILE_READ_DOUBLE_LEN 24	// len for reading double values from file
#define DB_FILE_READ_INIT_LEN 63	// init. len alloc. for reading column from file

/* Available types */
typedef enum db_type { INTEGER=0, DOUBLE, STRING } db_type;

/* Column definition */
typedef struct db_col {
	char name[DB_COL_MAX_NAME_LEN];
	char comment[DB_COL_MAX_COMMENT_LEN];
	int type;
	bool mandatory;
	bool unique;
} db_col;

/*
 * Main database structure
 * types and lines are reallocated if needed when adding.
 */
typedef struct db_base {
	char name[DB_MAX_NAME_LEN];
	char comment[DB_MAX_COMMENT_LEN];
	int current_id;
	parray *cols;	// array of db_col
	parray *lines;	// array of array of void *
} db_base;


/*
 * memory allocations
 */
db_base *db_new(size_t a_cols, size_t a_lines, char *name, char *comment);
db_base *db_new_with_id(size_t a_cols, size_t a_lines, char *name, char *comment);
db_base *db_col_add(db_base *db, bool mandatory, bool unique, int type, char *name, char *comment);
parray *db_line_new(db_base *db);
parray *db_line_new_with_id(db_base *db);


/*
 * Accessors
 */
int db_get_type(db_base *db, size_t c);
parray *db_get(db_base *db, int id);
parray *db_find(db_base *db, parray *search, int (*cmp)(void *, void *));
parray *db_find_all(db_base *db, parray *search, int (*cmp)(void *, void *), size_t init_alloc);
parray *db_find_by_id(db_base *db, int id);


/*
 * Helpers
 */
int db_cmp_id(void *id1, void *id2);


/*
 * Modifiers
 */
void db_set_cmp(db_base *db, int (*cmp)(void *, void *));
db_base *db_insert(db_base *res, parray *line);
/* set value by reference. In case of strings, set string->alloc_len */
db_base *db_line_set(db_base *db, parray *line, size_t c, int type, void *value, size_t alloc_len);
db_base *db_line_set_null(db_base *db, parray *line, size_t c, int type);
/* remove NULL lines */
void db_remove_nulls(db_base *db);
/* resize to a_lines * new_coef if n_lines < a_lines * coef */
db_base *db_resize(db_base *db, double coef, double new_coef);
void db_delete(db_base *db, size_t l, bool keep_null);


/*
 * free functions
 */
void db_line_col_free(db_base *db, parray *line, size_t c);
void db_line_free(db_base *db, parray *line);
void db_free(db_base *db);


/*
 * Printing
 */
void db_write(db_base *db, FILE *file);
void db_line_write(db_base *db, FILE *file, size_t l);

/*
 * Files manipulation
 */
db_base *db_file_load(db_base *db, FILE *file);


