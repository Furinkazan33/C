/*
	Base de donnees avec memoire contigue.
	Une ligne en table est de type char * qui commence par un champs id de type int.
	Chaque colonne peut avoir un index qui est une hashtable (valeur colonne => pointeur sur la ligne).
*/

/* TODOs:
	- add types DT_FLOAT, DT_INT64
*/


#include <stdio.h>
#include <stdbool.h>
#include "ht.h"
#include "macros.h"



/*
 *
 *	db
 *
 */

#define TABLE_NAME_LEN 40
#define TABLE_COLNAME_LEN 36
#define RESULT_NAME_LEN 64
#define TABLE_DEFAULT_CAP 20
#define TABLE_DEFAULT_COLS_CAP 5

// for reading type as string from file
#define TABLE_TYPE_LEN 10
#define TABLE_DATA_DELIM ";"
#define TABLE_INTEGER_AS_STRING_LEN 20

#define TABLE_PARAM_NAME_LEN 64
#define TABLE_PARAM_VALUE_LEN 64

/*
*	parameters used for reading tables schemas
*/
typedef struct param {
	char name[TABLE_PARAM_NAME_LEN];
	char value[TABLE_PARAM_VALUE_LEN];
} param;


/*
*	data types
*/
typedef enum data_type {
	DT_ID, DT_INT32, // same size
	DT_INT16,
	DT_INT64,
	DT_CHAR, 
	DT_TEXT,		// any length, specified at column creation
} data_type;

const char *data_type_tos(data_type t);
int string_to_data_type(char *type);
int data_type_to_size(data_type type, size_t len);



/*
*	columns
*/

typedef struct column {
	char name[TABLE_COLNAME_LEN];
	data_type type;
	size_t size;
	hashtable *index;
} column;

// may truncate name
column *column_new(char *name, data_type type, int len);



/*
*	tables
*/
typedef enum table_type {
	TT_TABLE,
	TT_RESULT,
} table_type;

const char *table_type_tos(table_type type);
table_type string_to_table_type(char *type);

typedef struct table {
	int id;
	int line_id;
	table_type type;
	char name[TABLE_NAME_LEN];
	int line_size;
	int cols_cnt;
	int cols_cap;
	int cap;
	int cnt;
	column **columns;
	char **lines;
} table;

// may truncate name, use id if > 0
table *table_new(table_type type, int id, int line_id, const char *name);
void table_free(table *t, bool free_lines);

table *table_add_col(table *t, char *name, data_type type, size_t size_for_text, size_t partitions);
table *table_add_col_id(table *t, char *name, size_t partitions);
table *table_add_col_int16(table *t, char *name, size_t partitions);
table *table_add_col_int32(table *t, char *name, size_t partitions);
table *table_add_col_char(table *t, char *name, size_t partitions);
table *table_add_col_text(table *t, char *name, size_t size, size_t partitions);

table *table_add_index(table *t, int col_idx, size_t partitions);
table *table_add_index_by_colname(table *t, char *colname, size_t partitions);
table *table_add_line(table *t, char *line);

// every line creation duplicates datas
char *line_new(table *t, char *datas[]);
char *line_new_correct_types(table *t, void *datas[]);
char *line_new_with_id(table *t, char *datas[]);
char *line_new_with_id_correct_types(table *t, void *datas[]);
char *line_get_data_ptr(table *t, char *line, int col);

// getting line
char **table_get_line_ptr_by(table *t, const char *colname, void *value, size_t *n_found);
char **table_get_line_ptr_by_value_char(table *t, const char *colname, char value, size_t *n_found);
char **table_get_line_ptr_by_value_int16(table *t, const char *colname, int16_t value, size_t *n_found);
char **table_get_line_ptr_by_value_int32(table *t, const char *colname, int32_t value, size_t *n_found);
char **table_get_line_ptr_by_value_int64(table *t, const char *colname, int64_t value, size_t *n_found);
char **table_get_line_ptr_by_value_string(table *t, const char *colname, char *value, size_t *n_found);
char *table_get_line_ptr_by_id(table *t, int id);

// getting data
void **table_get_data_ptr_by(table *t, const char *colname, void *value, const char *colname_target, size_t *n_found);
void **table_get_data_ptr_by_value_char(table *t, const char *colname, char value, const char *colname_target, size_t *n_found);
void **table_get_data_ptr_by_value_int16(table *t, const char *colname, int16_t value, const char *colname_target, size_t *n_found);
void **table_get_data_ptr_by_value_int32(table *t, const char *colname, int32_t value, const char *colname_target, size_t *n_found);
void **table_get_data_ptr_by_value_int64(table *t, const char *colname, int64_t value, const char *colname_target, size_t *n_found);
void **table_get_data_ptr_by_value_string(table *t, const char *colname, char *value, const char *colname_target, size_t *n_found);
void *table_get_data_ptr_by_id(table *t, int id, const char *colname);

typedef table result;
// getting line as a result table
result *table_get_line_by(table *t, const char *colname, void *value, bool make_copy);
result *table_get_line_by_value_char(table *t, const char *colname, char value, bool make_copy);
result *table_get_line_by_value_int16(table *t, const char *colname, int16_t value, bool make_copy);
result *table_get_line_by_value_int32(table *t, const char *colname, int32_t value, bool make_copy);
result *table_get_line_by_value_int64(table *t, const char *colname, int64_t value, bool make_copy);
result *table_get_line_by_value_string(table *t, const char *colname, char *value, bool make_copy);
result *table_get_line_by_id(table *t, int id, bool make_copy);

// getting data as a result table
result *table_get_data_by(table *t, const char *colname, void *value, const char *colname_target);
result *table_get_data_by_value_char(table *t, const char *colname, char value, const char *colname_target);
result *table_get_data_by_value_int16(table *t, const char *colname, int16_t value, const char *colname_target);
result *table_get_data_by_value_int32(table *t, const char *colname, int32_t value, const char *colname_target);
result *table_get_data_by_value_int64(table *t, const char *colname, int64_t value, const char *colname_target);
result *table_get_data_by_value_string(table *t, const char *colname, char *value, const char *colname_target);
result *table_get_data_by_id(table *t, int id, const char *colname);




/*
*	IO
*/
char *line_fprintf_readable(FILE *file, table *t, char *line, int col_width);
table *table_fprintf_readable(FILE *file, table *t, int col_width);
void table_fprintf_readable_schema(FILE *file, table *t);

table *table_write(FILE *file, table *t);
table *table_read(FILE *file);

table *table_new_from_schema(FILE *file);
