#include "db.h"
#include <inttypes.h>

#define DB_WRITE_COL_SIZE sizeof(column) - sizeof(hashtable *)
#define DB_WRITE_TABLE_SIZE sizeof(table) - sizeof(column **) - sizeof(char **)

static int _table_id = 0;
static int _result_id = 0;




/*
 *
 *	data_type
 *
 */

const char *data_type_tos(data_type t) {
	switch(t) {
		case DT_ID:
			return "ID";
		case DT_INT16:
			return "INT16";
		case DT_INT32:
			return "INT32";
		case DT_INT64:
			return "INT64";
		case DT_CHAR:
			return "CHAR";
		case DT_TEXT:
		default:
			return "TEXT";
	}
}

int string_to_data_type(char *type) {
	if(!strcmp(type, "ID")) {
		return DT_ID;
	}
	else if(!strcmp(type, "INT16")) {
		return DT_INT16;
	}
	else if(!strcmp(type, "INT32")) {
		return DT_INT32;
	}
	else if(!strcmp(type, "INT64")) {
		return DT_INT64;
	}
	else if(!strcmp(type, "CHAR")) {
		return DT_CHAR;
	}
	else if(!strcmp(type, "TEXT")) {
		return DT_TEXT;
	}
	else {
		LOG_E("unexpected type %s\n", type);
		return -1;
	}
}

int data_type_to_size(data_type type, size_t size) {
	switch(type) {
		case DT_ID:
		case DT_INT32:
			return 4;
		case DT_INT64:
			return 8;
		case DT_INT16:
			return 2;
		case DT_CHAR:
			return 1;
		case DT_TEXT:
			return size;
		default:
			LOG_E("unexpected type: %d\n", type);
			return -1;
	}
}

typedef int64_t cmp_fun(void *, void *);

int64_t cmp_char (void *v1, void *v2) {
	return *(char *)v1 - *(char *)v2;
}

int64_t cmp_int16 (void *v1, void *v2) {
	return *(int16_t *)v1 - *(int16_t *)v2;
}

int64_t cmp_int32 (void *v1, void *v2) {
	return *(int32_t *)v1 - *(int32_t *)v2;
}

int64_t cmp_int64 (void *v1, void *v2) {
	return *(int64_t *)v1 - *(int64_t *)v2;
}

int64_t cmp_string (void *v1, void *v2) {
	return strcmp((char *)v1,  (char *)v2);
}

cmp_fun *get_cmp_function_for_type(data_type type) {
	cmp_fun *res = NULL;
	
	switch(type) {
		case DT_CHAR:
			res = cmp_char;
			break;
		case DT_INT16:
			res = cmp_int16;
			break;
		case DT_ID:
		case DT_INT32:
			res = cmp_int32;
			break;
		case DT_INT64:
			res = cmp_int64;
			break;
		case DT_TEXT:
			res = cmp_string;
			break;
	}
	
	return res;
}




/*
 *
 *	column
 *
 */

column *column_new(char *name, data_type type, int size) {
	if(type < DT_ID || type > DT_TEXT) {
		LOG_E("unexpected type:[%d]\n", type)
		return NULL;
	}
	
	column *res;
	MALLOC(res, sizeof(*res), return NULL, )
	
	strncpy(res->name, name, TABLE_COLNAME_LEN);
	res->name[TABLE_COLNAME_LEN] = 0;
	res->type = type;
	res->size = size;
	res->index = NULL;
	
	return res;
}

size_t col_get_offset(table *t, int col_idx) {
	size_t offset = 0;
	for(int c = 0; c < col_idx; c++) {
		offset += t->columns[c]->size;
	}
	
	return offset;
}

int get_col_idx_by_name(table *t, const char *colname) {	
	for(int i = 0; i < t->cols_cnt; i++) {
		if(!strcmp(colname, t->columns[i]->name)) {
			return i;
		}
	}
	
	// not found
	return -1;
}




/*
 *
 *	line
 *
 */
// datas contain pointers to any types
char *line_new_with_id(table *t, char *datas[]) {
	char *res;
	MALLOC(res, t->line_size, return NULL, )
	
	size_t offset = 0;
	
	int16_t value_int16;
	int32_t value_int32;
	int64_t value_int64;
	
	// adding rest from datas after conversion
	for(int c = 0; c < t->cols_cnt; c++) {
		size_t size = t->columns[c]->size;
		
		switch(t->columns[c]->type) {
			case DT_ID:
			case DT_INT32:
				value_int32 = atoi(datas[c]);
				memcpy(res + offset, &value_int32, size);
				break;
			case DT_INT16:
				value_int16 = atoi(datas[c]);
				memcpy(res + offset, &value_int16, size);
				break;
			case DT_INT64:
				value_int64 = strtoll(datas[c], NULL, 10);
				memcpy(res + offset, &value_int64, size);
				break;
			case DT_CHAR:
				memcpy(res + offset, datas[c], size);
				break;
			case DT_TEXT:
				strncpy(res + offset, datas[c], size);
				break;
		}
		offset += size;
	}
	
	return res;
}

char *line_new_with_id_correct_types(table *t, void *datas[]) {
	char *res;
	MALLOC(res, t->line_size, return NULL, )
	
	size_t offset = 0;
	
	for(int c = 0; c < t->cols_cnt; c++) {
		size_t size = t->columns[c]->size;
		
		memcpy(res + offset, datas[c], size);
		
		offset += size;
	}
	
	return res;
}

char *line_copy(table *t, char *line) {
	char *res;
	MALLOC(res, t->line_size, LOG_E_MESS("failed to alloc new line\n"); return NULL, )
	memcpy(res, line, t->line_size);
	
	return res;
}

char *line_get_data_ptr(table *t, char *line, int col_idx) {
	return line + col_get_offset(t, col_idx);
}

char *line_new(table *t, char *datas[]) {
	char *res;
	MALLOC(res, t->line_size, return NULL, )
	
	size_t offset = 0;
	
	// addding id
	size_t size = t->columns[0]->size;
	t->line_id++;
	*((int *) (res + offset)) = t->line_id;
	offset += size;
	
	// adding rest from datas after conversion
	for(int c = 0; c < t->cols_cnt - 1; c++) {
		size = t->columns[c + 1]->size;
		
		switch(t->columns[c + 1]->type) {
			case DT_INT16:
				*((int16_t *) (res + offset)) = atoi(datas[c]);
				break;
			case DT_ID:
			case DT_INT32:
				*((int32_t *) (res + offset)) = atoi(datas[c]);
				break;
			case DT_INT64:
				*((int64_t *) (res + offset)) = strtoll(datas[c], NULL, 10);
				break;
			case DT_CHAR:
				memcpy(res + offset, datas[c], size);
				break;
			case DT_TEXT:
				strncpy(res + offset, datas[c], size);
				break;
		}
		offset += size;
	}
	
	return res;
}

char *line_new_correct_types(table *t, void *datas[]) {
	char *res;
	
	MALLOC(res, t->line_size, return NULL, )
	size_t offset = 0;
	
	// addding id
	t->line_id++;
	*((int *) res) = t->line_id;
	offset += t->columns[0]->size;
	
	// adding rest from datas - no conversion
	for(int c = 0; c < t->cols_cnt - 1; c++) {
		size_t size = t->columns[c + 1]->size;
		
		memcpy(res + offset, datas[c], size);

		offset += size;
	}
	
	return res;
}




/*
 *
 *	table
 *
 */

table *table_new(table_type type, int32_t id, int line_id, const char *name) {
	table *res;
	MALLOC(res, sizeof(*res), return NULL, )
	
	if(id > 0) {
		res->id = id;
	}
	else {
		switch(type) {
			case TT_TABLE:
				res->id = ++_table_id;
				break;
			
			case TT_RESULT:
				res->id = ++_result_id;
				break;
		}
	}
	
	if(line_id >= 0) {
		res->line_id = line_id;
	}
	else {
		res->line_id = 0;
	}
	
	strncpy(res->name, name, TABLE_NAME_LEN);
	res->name[TABLE_NAME_LEN] = 0;
	
	res->type = type;
	
	res->line_size = 0;
	
	res->cols_cap = TABLE_DEFAULT_COLS_CAP;
	res->cols_cnt = 0;
	MALLOC(res->columns, sizeof(*(res->columns)) * res->cols_cap, return NULL, )
	
	res->cap = TABLE_DEFAULT_CAP;
	res->cnt = 0;
	MALLOC(res->lines, sizeof(*(res->lines)) * res->cap, return NULL, )
	
	return res;
}

void table_free(table *t, bool free_lines) {
	ARRAY_FREE(t->columns, (size_t) t->cols_cnt);
	
	if(free_lines) {
		for(int l = 0; l < t->cnt; l++) {
			free(t->lines[l]);
		}
	}
	
	free(t->lines);
	
	free(t);
}

table *table_add_index(table *t, int col_idx, size_t partitions) {
	column *col = t->columns[col_idx];
	
	if(col->index) {
		hashtable_free(col->index);
	}
	
	MALLOC(col->index, sizeof(*(col->index)), return NULL, )

	switch(col->type) {
		case DT_ID:
		case DT_INT32:
			col->index = hashtable_new(HT_INT32, partitions); if(!col->index) { LOG_E_MESS("failed to create index\n"); return NULL; }
			break;
		case DT_INT16:
			col->index = hashtable_new(HT_INT16, partitions); if(!col->index) { LOG_E_MESS("failed to create index\n"); return NULL; }
			break;
		case DT_INT64:
			col->index = hashtable_new(HT_INT64, partitions); if(!col->index) { LOG_E_MESS("failed to create index\n"); return NULL; }
			break;
		case DT_CHAR:
			col->index = hashtable_new(HT_CHAR, partitions); if(!col->index) { LOG_E_MESS("failed to create index\n"); return NULL; }
			break;
		case DT_TEXT:
			col->index = hashtable_new(HT_STR, partitions); if(!col->index) { LOG_E_MESS("failed to create index\n"); return NULL; }
			break;
	}
	
	size_t col_offset = col_get_offset(t, col_idx);
	
	for(int l = 0; l < t->cnt; l++) {
		if(!hashtable_add(col->index, t->lines[l] + col_offset, t->lines[l])) {
			LOG_E("failed to add line")
			return NULL;
		}
	}
	
	return t;
}

table *table_add_index_by_colname(table *t, char *colname, size_t partitions) {
	int col_idx = get_col_idx_by_name(t, colname);
	if(col_idx < 0) {
		LOG_E("ERROR: no such column:%s\n", colname); 
		return NULL;
	}
	
	return table_add_index(t, col_idx, partitions);
}

table *_table_realloc_cols(table *t, int coef) {
	#ifdef DB_DEBUG
		LOG_S("DEBUG: realloc cols. %d => %d\n", t->cols_cap, t->cols_cap * coef);
	#endif
	
	t->cols_cap *= coef;
	REALLOC(t->columns, sizeof(*(t->columns)) * t->cols_cap, return NULL, )
	
	return t;
}

table *table_add_col(table *t, char *name, data_type type, size_t size_for_text, size_t partitions) {
	int size = data_type_to_size(type, size_for_text);
	
	if(size == -1) { LOG_E_MESS("failed to get data size\n"); return NULL; }
	
	column *c = column_new(name, type, size); if(!c) { LOG_E_MESS("failed to create col\n"); return NULL; }
	
	if(t->cols_cnt == t->cols_cap) {
		if(!_table_realloc_cols(t, 2)) return NULL;
	}

	t->columns[t->cols_cnt] = c;
	
	t->line_size += size;
	
	//realloc lines
	for(int l = 0; l < t->cnt; l++) {
		REALLOC(t->lines[l], t->line_size, return NULL, )
	}
	
	if(partitions > 0) {
		if(!table_add_index(t, t->cols_cnt, partitions)) { LOG_E_MESS("failed to add index\n"); return NULL; };
	}
	
	t->cols_cnt++;
	
	return t;
}

table *table_add_col_id(table *t, char *name, size_t partitions) {
	return table_add_col(t, name, DT_ID, 0, partitions);
}

table *table_add_col_int16(table *t, char *name, size_t partitions) {
	return table_add_col(t, name, DT_INT16, 0, partitions);
}

table *table_add_col_int32(table *t, char *name, size_t partitions) {
	return table_add_col(t, name, DT_INT32, 0, partitions);
}

table *table_add_col_int64(table *t, char *name, size_t partitions) {
	return table_add_col(t, name, DT_INT64, 0, partitions);
}

table *table_add_col_char(table *t, char *name, size_t partitions) {
	return table_add_col(t, name, DT_CHAR, 0, partitions);
}

table *table_add_col_text(table *t, char *name, size_t size, size_t partitions) {
	return table_add_col(t, name, DT_TEXT, size, partitions);
}


table *_table_realloc_lines(table *t, int coef) {
	t->cap *= coef;
	REALLOC(t->lines, sizeof(*(t->lines)) * t->cap, return NULL, return t)
}

table *table_add_line(table *t, char *line) {
	#ifdef DB_DEBUG
		LOG_S("DEBUG: adding line with id=[%d]\n", *(int32_t *) line);
	#endif
	
	
	if(t->cnt == t->cap) {
		if(!_table_realloc_lines(t, 2)) { LOG_E("failedf to realloc"); return NULL; }
	}

	t->lines[t->cnt] = line;
	t->cnt++;
	
	// adding to existing indexes
	for(int c = 0; c < t->cols_cnt; c++) {
		if(t->columns[c]->index) {
			#ifdef DB_DEBUG
				LOG_S("DEBUG: column [%s] has an index\n", t->columns[c]->name)
			#endif
	
			size_t offset = col_get_offset(t, c);
			
			#ifdef DB_DEBUG
				LOG_S("DEBUG: column [%d], offset=[%ld]\n", c, offset);
			#endif
			
			if(!hashtable_add(t->columns[c]->index, line + offset, line)) {
				LOG_E("failed to add to column [%s] index \n", t->columns[c]->name);
				return NULL;
			}
		}
	}
	
	return t;
}

/* 
	getting lines
*/
char **table_get_line_ptr_by(table *t, const char *colname, void *value, size_t *n_found) {
	char **found;
	
	int col_idx = get_col_idx_by_name(t, colname);
	if(col_idx < 0) {
		LOG_E("ERROR: no such column:%s\n", colname); 
		return NULL;
	}
	#ifdef DB_DEBUG
		LOG_S("DEBUG: col_idx for [%s]=[%d]\n", colname, col_idx);
	#endif
	
	// index on col
	if(t->columns[col_idx]->index != NULL) {
		#ifdef DB_DEBUG
			LOG_S_MESS("DEBUG: getting line from index\n")
		#endif
		
		found = (char **) hashtable_find(t->columns[col_idx]->index, value, true, n_found, 32); if(!found) { LOG_E_MESS("ERROR: failed to search in hashtable\n"); return NULL; }
		
		return found;
	}
	else {
		#ifdef DB_DEBUG
			LOG_S_MESS("DEBUG: no index\n")
		#endif
		
		cmp_fun *cmp = get_cmp_function_for_type(t->columns[col_idx]->type);
		
		size_t found_alloc = 32;
		MALLOC(found, sizeof(char *) * found_alloc, LOG_E_MESS("failed to alloc result\n"); return NULL;, )
		
		
		// looking for value
		*n_found = 0;
		for(int i = 0; i < t->cnt; i++) {
			char *line = t->lines[i];
			
			if(!cmp((line_get_data_ptr(t, line, col_idx)), value)) {
				// realloc
				if(*n_found == found_alloc) {
					found_alloc *= 2;
					REALLOC(found, sizeof(char *) * found_alloc, LOG_E_MESS("failed to realloc result\n"); return NULL;, )
				}
				
				found[(*n_found)++] = line;
			}
		}
	}
	
	return found;
}

char **table_get_line_ptr_by_value_int(table *t, const char *colname, int value, size_t *n_found) {
	return table_get_line_ptr_by(t, colname, &value, n_found);
}

char **table_get_line_ptr_by_value_string(table *t, const char *colname, char *value, size_t *n_found) {
	return table_get_line_ptr_by(t, colname, value, n_found);
}

char *table_get_line_ptr_by_id(table *t, int32_t id) {
	size_t n_found;
	char **found = table_get_line_ptr_by_value_int(t, "id", id, &n_found); if(!found) { LOG_E_MESS("failed to search lines\n"); return NULL; }
	if(n_found != 1) { LOG_E_MESS("search by id returns more than 1 result\n"); return NULL; }
	
	char *res = found[0];
	
	free(found);
	
	return res;
}

/* 
	getting data
*/
void **table_get_data_ptr_by(table *t, const char *colname, void *value, const char *colname_target, size_t *n_found) {
	char **found = table_get_line_ptr_by(t, colname, value, n_found);
	
	int col_idx = get_col_idx_by_name(t, colname_target);
	if(col_idx < 0) { LOG_E("ERROR: no such column:%s\n", colname_target); return NULL; }
	
	void **res;
	MALLOC(res, sizeof(*res) * *n_found, LOG_E_MESS("failed to alloc results\n"); return NULL, )
	
	for(size_t i = 0; i < *n_found; i++) {
		res[i] = line_get_data_ptr(t, found[i], col_idx);
	}
	
	free(found);
	
	return res;
}

void **table_get_data_ptr_by_value_int(table *t, const char *colname, int value, const char *colname_target, size_t *n_found) {
	char **found = table_get_line_ptr_by_value_int(t, colname, value, n_found);
	
	int col_idx = get_col_idx_by_name(t, colname_target);
	if(col_idx < 0) { LOG_E("ERROR: no such column:%s\n", colname_target); return NULL; }
	
	void **res;
	MALLOC(res, sizeof(*res) * *n_found, LOG_E_MESS("failed to alloc results\n"); return NULL, )
	
	for(size_t i = 0; i < *n_found; i++) {
		res[i] = line_get_data_ptr(t, found[i], col_idx);
	}
	
	free(found);
	
	return res;
}

void **table_get_data_ptr_by_value_string(table *t, const char *colname, char *value, const char *colname_target, size_t *n_found) {
	char **found = table_get_line_ptr_by_value_string(t, colname, value, n_found);
	
	int col_idx = get_col_idx_by_name(t, colname_target);
	if(col_idx < 0) { LOG_E("ERROR: no such column:%s\n", colname_target); return NULL; }
	
	void **res;
	MALLOC(res, sizeof(*res) * *n_found, LOG_E_MESS("failed to alloc results\n"); return NULL, )
	
	for(size_t i = 0; i < *n_found; i++) {
		res[i] = line_get_data_ptr(t, found[i], col_idx);
	}
	
	free(found);
	
	return res;
}

void *table_get_data_ptr_by_id(table *t, int32_t id, const char *colname) {
	size_t n_found;
	void **found = table_get_data_ptr_by_value_int(t, "id", id, colname, &n_found);
	
	if(n_found != 1) { LOG_E_MESS("search by id returns more than 1 result\n"); return NULL; }
	
	void *res = found[0];
	
	free(found);
	
	return res;
}



/*
 *
 *	result - for table requests
 *
 */

/* 
	getting line
*/
result *table_get_line_by(table *t, const char *colname, void *value, bool make_copy) {
	#ifdef DB_DEBUG
		LOG_S("DEBUG: table_get_line_by\n");
	#endif
	
	// creating result table
	result *res = table_new(TT_RESULT, -1, -1, __func__);

	// adding columns
	for(int c = 0; c < t->cols_cnt; c++) {
		if(!table_add_col(res, t->columns[c]->name, t->columns[c]->type, t->columns[c]->size, 0)) { table_free(res, true); LOG_E_MESS("failed to add col\n"); return NULL; }
	}

	// getting line
	size_t n_found;
	char **lines = table_get_line_ptr_by(t, colname, value, &n_found); if(!lines) { table_free(res, true); LOG_E_MESS("failed to search for lines\n"); return NULL; }
	
	#ifdef DB_DEBUG
		LOG_S("DEBUG: n_found:[%ld]\n", n_found);
	#endif
	
	for(size_t i = 0; i < n_found; i++) {
		char *line_result = lines[i];

		if(make_copy) {
			line_result = line_copy(res, line_result); if(!line_result) { table_free(res, true); LOG_E_MESS("failed to copy lines\n"); return NULL; }
		}
		
		if(!table_add_line(res, line_result)) { table_free(res, true); LOG_E("failed to add lines"); return NULL; }
	}

	// even if empty
	return res;
}

result *table_get_line_by_value_int16(table *t, const char *colname, int16_t value, bool make_copy) {
	return table_get_line_by(t, colname, &value, make_copy);
}

result *table_get_line_by_value_int32(table *t, const char *colname, int32_t value, bool make_copy) {
	return table_get_line_by(t, colname, &value, make_copy);
}

result *table_get_line_by_value_int64(table *t, const char *colname, int64_t value, bool make_copy) {
	return table_get_line_by(t, colname, &value, make_copy);
}

result *table_get_line_by_value_char(table *t, const char *colname, char value, bool make_copy) {
	return table_get_line_by(t, colname, &value, make_copy);
}

result *table_get_line_by_value_string(table *t, const char *colname, char *value, bool make_copy) {
	return table_get_line_by(t, colname, value, make_copy);
}

result *table_get_line_by_id(table *t, int32_t id, bool make_copy) {
	return table_get_line_by_value_int32(t, "id", id, make_copy);
}

/* 
	getting data
*/
result *table_get_data_by(table *t, const char *colname, void *value, const char *colname_target) {
	// creating result table
	result *res = table_new(TT_RESULT, -1, -1, __func__);
	
	if(!table_add_col(res, "id", DT_ID, 0, 0)) { LOG_E_MESS("failed to add col\n"); table_free(res, true); return NULL; }
	
	// adding column
	int col_idx = 0;
	
	if(colname_target) {
		col_idx = get_col_idx_by_name(t, colname_target);
		if(col_idx < 0) {
			table_free(res, true); 
			LOG_E("ERROR: no such column:%s\n", colname_target); 
			return res;
		}
		
		if(!table_add_col(res, t->columns[col_idx]->name, t->columns[col_idx]->type, t->columns[col_idx]->size, 0)) { table_free(res, true); LOG_E_MESS("failed to add col\n"); return NULL; }
	}
	
	// getting line
	size_t n_found;
	char **lines = table_get_line_ptr_by(t, colname, value, &n_found); if(!lines) { table_free(res, true); return NULL; }
	
	for(size_t i = 0; i < n_found; i++) {
		if(colname_target) {
			// creating the result line with (id, colname_target) and adding to result table
			if(!table_add_line(res, line_new_with_id_correct_types(res, (void *[]) { lines[i], line_get_data_ptr(t, lines[i], col_idx) }))) { 
				table_free(res, true); 
				LOG_E("failed to add line\n"); 
				return NULL; 
			}
		}
		else {
			// creating the result line with (id) and adding to result table
			if(!table_add_line(res, line_new_with_id_correct_types(res, (void *[]) { lines[i] }))) { 
				table_free(res, true); 
				LOG_E("failed to add line\n"); 
				return NULL; 
			}
		}
	}
	
	// even if empty
	return res;
}

result *table_get_data_by_value_char(table *t, const char *colname, char value, const char *colname_target) {
	return table_get_data_by(t, colname, &value, colname_target);
}

result *table_get_data_by_value_int16(table *t, const char *colname, int16_t value, const char *colname_target) {
	return table_get_data_by(t, colname, &value, colname_target);
}

result *table_get_data_by_value_int32(table *t, const char *colname, int32_t value, const char *colname_target) {
	return table_get_data_by(t, colname, &value, colname_target);
}

result *table_get_data_by_value_int64(table *t, const char *colname, int64_t value, const char *colname_target) {
	return table_get_data_by(t, colname, &value, colname_target);
}

result *table_get_data_by_value_string(table *t, const char *colname, char *value, const char *colname_target) {
	return table_get_data_by(t, colname, value, colname_target);
}

result *table_get_data_by_id(table *t, int32_t id, const char *colname) {
	return table_get_data_by_value_int32(t, "id", id, colname);
}




/*
 *
 *	IO operations - save / load
 *
 */

// loading a table after a dump
table *table_read(FILE *file) {
	// read and create table
	size_t read_size = DB_WRITE_TABLE_SIZE + sizeof(char);
	char lbuffer[read_size];
	table *tinfos = (table *) lbuffer;
	fread(lbuffer, read_size, 1, file);
	table *res = table_new(tinfos->type, tinfos->id, tinfos->line_id, tinfos->name);
	
	// read and add columns to table
	read_size = DB_WRITE_COL_SIZE + sizeof(size_t) + sizeof(char);
	char cbuffer[read_size];
	column *cinfos = (column *) cbuffer;
	
	for(int i = 0; i < tinfos->cols_cnt; i++) {
		fread(cbuffer, read_size, 1, file);
		if(!table_add_col(res, cinfos->name, cinfos->type, cinfos->size, *(&(cinfos->size) + 1))) { LOG_E_MESS("failed to add col\n"); return NULL; }
	}
	
	// read and add lines to table 
	for(int i = 0; i < tinfos->cnt; i++) {
		char *line;
		MALLOC(line, res->line_size + 1, return NULL, )
		fread(line, res->line_size + 1, 1, file);
		table_add_line(res, line);
	}
		
	return res;
}

// dumping to a file
table *table_write(FILE *file, table *t) {
	char line_return = '\n';
	
#ifdef DEBUG
	LOG_E("writing table\n");
#endif

	// write table
	fwrite(t, DB_WRITE_TABLE_SIZE, 1, file);
	fwrite(&line_return, sizeof(char), 1, file);

#ifdef DEBUG
	LOG_E("writing columns\n");
#endif

	// write columns
	for(int i = 0; i < t->cols_cnt; i++) {
		size_t partitions = 0;
		if(t->columns[i]->index) {
			partitions = t->columns[i]->index->partitions;
		}
		fwrite(t->columns[i], DB_WRITE_COL_SIZE, 1, file);
		fwrite(&partitions, sizeof(partitions), 1, file);
		fwrite(&line_return, sizeof(char), 1, file);
	}

#ifdef DEBUG
	LOG_E("writing lines\n");
#endif

	// write lines
	for(int l = 0; l < t->cnt; l++) {
		fwrite(t->lines[l], t->line_size, 1, file); 
		fwrite(&line_return, sizeof(char), 1, file);
	}

	return t;
}




/*
 *
 *	IO operations - human readable
 *
 */

char *line_fprintf_readable(FILE *file, table *t, char *line, int col_width) {
	char int_as_str[col_width];
	char *sdata;

	for(int c = 0; c < t->cols_cnt; c++) {
		char value[col_width];
		int size = t->columns[c]->size;
		
		switch(t->columns[c]->type) {
			case DT_INT16:
				sprintf(int_as_str, "%d", *((int16_t *) line));
				strncpy(value, int_as_str, col_width);
				value[col_width] = 0;
				fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, value);
				break;
			case DT_ID:
			case DT_INT32:
				sprintf(int_as_str, "%d", *((int32_t *) line));
				strncpy(value, int_as_str, col_width);
				value[col_width] = 0;
				fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, value);
				break;
			case DT_INT64:
				sprintf(int_as_str, "%" PRId64 "", *((int64_t *) line));
				strncpy(value, int_as_str, col_width);
				value[col_width] = 0;
				fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, value);
				break;
			case DT_CHAR:
				value[0] = '\'';
				value[1] = line[0];
				value[2] = '\'';
				value[3] = 0;
				fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, value);
				break;
			case DT_TEXT:
				MALLOC(sdata, size + 1, return NULL, )
					strncpy(sdata, line, size);
				sdata[size] = 0;
				value[0] = '"';
				value[1] = 0;
				strcat(value, sdata);
				free(sdata);

				strcat(value, "\"");
				value[col_width] = 0;
				fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, value);
				break;
		}

		line += t->columns[c]->size;
	}
	fprintf(file, "\n");

	return line;
}

void _line_fprintf_readable_separator(FILE *file, table *t, int col_width) {
	char sep[col_width + 2];
	memset(sep, '-', col_width + 1);
	sep[col_width + 1] = 0;
	for(int i = 0; i < t->cols_cnt; i++) {	
		fprintf(file, "%s", sep);
	}
	fprintf(file, "\n");
}

const char *_table_type_tos(table_type type) {
	const char *res = NULL;
	
	switch(type) {
		case TT_TABLE:
			res = "table";
			break;
		case TT_RESULT:
			res = "result";
			break;
	}
	return res;
}

table *table_fprintf_readable(FILE *file, table *t, int col_width) {
	_line_fprintf_readable_separator(file, t, col_width);

	fprintf(file, "%s[id=%d][name=%s][cols=%d][lines=%d]\n", _table_type_tos(t->type), t->id, t->name, t->cols_cnt, t->cnt);

	_line_fprintf_readable_separator(file, t, col_width);

	// columns types
	for(int i = 0; i < t->cols_cnt; i++) {
		char type[col_width];
		strncpy(type, data_type_tos(t->columns[i]->type), col_width);
		type[col_width] = 0;
		fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, type);
	}
	fprintf(file, "\n");

	// columns real sizes
	for(int i = 0; i < t->cols_cnt; i++) {
		fprintf(file, "%*zu" TABLE_DATA_DELIM, col_width, t->columns[i]->size);
	}
	fprintf(file, "\n");

	// columns indexes
	for(int i = 0; i < t->cols_cnt; i++) {
		size_t partitions = 0;
		if(t->columns[i]->index) {
			partitions = t->columns[i]->index->partitions;
		}
		fprintf(file, "%*zu" TABLE_DATA_DELIM, col_width, partitions);
	}
	fprintf(file, "\n");

	// columns names
	for(int i = 0; i < t->cols_cnt; i++) {
		char name[col_width];
		strncpy(name, t->columns[i]->name, col_width);
		name[col_width] = 0;
		fprintf(file, "%*s" TABLE_DATA_DELIM, col_width, name);
	}
	fprintf(file, "\n");

	_line_fprintf_readable_separator(file, t, col_width);

	for(int l = 0; l <  t->cnt; l++) {
		if(!line_fprintf_readable(file, t, t->lines[l], col_width)) { return NULL; }		
	}

	_line_fprintf_readable_separator(file, t, col_width);

	return t;
}

void table_fprintf_readable_schema(FILE *file, table *t) {
	
	// table description
	fprintf(file, "%s[name=%s]\n", _table_type_tos(t->type), t->name);

	// columns descriptions
	for(int i = 0; i < t->cols_cnt; i++) {
		size_t partitions = 0;
		if(t->columns[i]->index) {
			partitions = t->columns[i]->index->partitions;
		}
		
		fprintf(file, "col[type=%s][size=%zu][partitions=%zu][name=%s]\n", 
			data_type_tos(t->columns[i]->type), 
			t->columns[i]->size,
			partitions,
			t->columns[i]->name);
	}
}

char *file_readline_new_string(FILE *f) {
	char *res;
	size_t alloc_len = 128;
	MALLOC(res, sizeof(char) * (alloc_len + 1), LOG_E_MESS("failed to alloc string\n"), )
	
	int c;
	size_t read = 0;
	
	while((c = fgetc(f)) != EOF) {
		if(read == alloc_len) {
			alloc_len *= 2;
			REALLOC(res, sizeof(char) * (alloc_len + 1), LOG_E_MESS("failed to realloc string\n"), )
		}
		
		res[read++] = c;
		
		if(c == '\n') {
			break;
		}
	}
	
	res[read] = '\0';
	
	return res;
}

// read param and advance src if no error
param *_line_read_param(param *param, char **src) {
	size_t i;
	char *p = *src;
	
	if(*p != '[') { LOG_E_MESS("malformed param\n"); return NULL; }
	p++;
	
	for(i = 0; *p && *p != '=' && *p != '\n'; i++, p++) { 
		param->name[i] = *p;
	}
	param->name[i] = '\0';
	
	if(*p != '=') { LOG_E_MESS("malformed param\n"); return NULL; }
	p++;
	
	for(i = 0; *p && *p != ']' && *p != '\n'; i++, p++) { 
		param->value[i] = *p;
	}
	param->value[i] = '\0';
	
	if(*p != ']') { LOG_E_MESS("malformed param\n"); return NULL; }
	p++;
	
	#ifdef DB_DEBUG
		LOG_S("DEBUG: param read:[%s=%s]\n", param->name, param->value);
	#endif
	
	*src = p;
	
	return param;
}

table *table_new_from_schema(FILE *file) {
	param prm;
	
	/* 
		reading and creating table
		table[name=person]
	*/
	char *table_name;
	char *line = file_readline_new_string(file); if(!line) { LOG_E_MESS("failed to read line\n"); return NULL; }
	if(strncmp(line, "table", 5) != 0) { LOG_E("unknown line start:[%s]\n", line); free(line); return NULL; }
	char *p = line + 5;
	
	#ifdef DB_DEBUG
		LOG_S("DEBUG: line read:[%s]\n", line);
	#endif
	
	while(*p && *p != '\n') {
		if(!_line_read_param(&prm, &p)) { free(line); LOG_E_MESS("failed to read parameter\n"); return NULL; }
		
		if(!strcmp((&prm)->name, "name")) {
			table_name = (&prm)->value;
		}
		else {
			free(line);
			LOG_E("unrecognize parameter:[%s]\n", (&prm)->name); 
			return NULL;
		}
	}
	
	free(line);
	
	table *res = table_new(TT_TABLE, -1, -1, table_name); if(!res) { LOG_E("failed to create table [%s]\n", table_name); return NULL; }
	
	
	/* 
		reading and adding cols
		col[type=ID][size=4][partitions=20][name=id]
	*/
	data_type col_type;
	size_t col_size;
	size_t col_partitions;
	char *col_name;
	
	while(true) {
		line = file_readline_new_string(file); if(!line) { table_free(res, true); LOG_E_MESS("failed to read col line\n"); return NULL; }
		
		if(strncmp(line, "col", 3) != 0) {
			break;
		}
		else {
			p = line + 3;
		
			#ifdef DB_DEBUG
				LOG_S("DEBUG: col line read:[%s]\n", line);
			#endif
		
			// reading parameters	
			while(*p && *p != '\n') {
				if(!_line_read_param(&prm, &p)) { 
					free(line); 
					table_free(res, true);
					LOG_E_MESS("failed to read parameter\n"); 
					return NULL; 
				}
				
				if(!strcmp((&prm)->name, "type")) {
					col_type = string_to_data_type((&prm)->value);
				}
				else if(!strcmp((&prm)->name, "size")) {
					col_size = atoi((&prm)->value);
				}
				else if(!strcmp((&prm)->name, "partitions")) {
					col_partitions = atoi((&prm)->value);
				}
				else if(!strcmp((&prm)->name, "name")) {
					col_name = (&prm)->value;
				}
				else {
					free(line);
					table_free(res, true);
					LOG_E("unrecognize parameter:[%s]\n", (&prm)->name); 
					return NULL;
				}
			}

			free(line);
			
			if(!table_add_col(res, col_name, col_type, col_size, col_partitions)) { 
				table_free(res, true);
				LOG_E("failed to add col [%s]\n", col_name); 
				return NULL; 
			}
		}
	}
	
	return res;
}

