#include "db.h"


#define DB_WRITE_COL_SIZE sizeof(column) - sizeof(hashtable *)
#define DB_WRITE_TABLE_SIZE sizeof(table) - sizeof(column **) - sizeof(char **)


/*
 *
 *	data_type
 *
 */

const char *data_type_tos(data_type t) {
	switch(t) {
		case DT_ID:
			return "ID";
		case DT_INT:
			return "INT";
		case DT_INT16:
			return "DT_INT16";
		case DT_INT32:
			return "DT_INT32";
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
	else if(!strcmp(type, "INT")) {
		return DT_INT;
	}
	else if(!strcmp(type, "CHAR")) {
		return DT_CHAR;
	}
	else if(!strcmp(type, "TEXT")) {
		return DT_TEXT;
	}
	else {
		LOG_E("unexpected type %s\n", type)
		return -1;
	}
}

int data_type_to_size(data_type type, size_t size) {
	switch(type) {
		case DT_INT:
		case DT_INT32:
		case DT_ID:
			return 4;
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


/*
 *
 *	column
 *
 */

column *column_new(char *name, data_type type, int size) {
	if(type > DT_TEXT) {
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
 *	table
 *
 */

static int _table_id = 0;
static int _result_id = 0;


table *table_new(table_type type, int id, int line_id, const char *name) {
	table *res;
	MALLOC(res, sizeof(*res), return NULL, )
	
	if(id > 0) {
		res->id = id;
	}
	else {
		if(type == TT_TABLE) {
			res->id = ++_table_id;
		}
		else if(type == TT_RESULT) {
			res->id = ++_result_id;
		}
		else {
			LOG_E("no such table type: %d\n", type);
			return NULL;
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

void table_free(table *t) {
	ARRAY_FREE(t->columns, (size_t) t->cols_cnt);
	
	for(int l = 0; l < t->cnt; l++) {
		free(t->lines[l]);
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
		case DT_INT:
		case DT_INT32:
		case DT_INT16:
			col->index = hashtable_new(HT_INT, partitions);
			break;
		case DT_CHAR:
			col->index = hashtable_new(HT_CHAR, partitions);
			break;
		case DT_TEXT:
			col->index = hashtable_new(HT_STR, partitions);
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

table *_table_realloc_cols(table *t, int coef) {
	#ifdef DB_DEBUG
		LOG_S("DEBUG: realloc cols. %d => %d\n", t->cols_cap, t->cols_cap * coef);
	#endif
	
	t->cols_cap *= coef;
	REALLOC(t->columns, sizeof(*(t->columns)) * t->cols_cap, return NULL, )
	
	return t;
}

table *table_add_col(table *t, char *name, data_type type, size_t size_for_text, size_t partitions) {
	size_t size = data_type_to_size(type, size_for_text);
	
	column *c = column_new(name, type, size);
	
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
		table_add_index(t, t->cols_cnt, partitions);
	}
	
	t->cols_cnt++;
	
	return t;
}

table *_table_realloc_lines(table *t, int coef) {
	t->cap *= coef;
	REALLOC(t->lines, sizeof(*(t->lines)) * t->cap, return NULL, return t)
}

table *table_add(table *t, char *line) {
	#ifdef DB_DEBUG
		LOG_S("DEBUG: adding line:[%d]\n", *(int *) line);
	#endif
	
	
	if(t->cnt == t->cap) {
		if(!_table_realloc_lines(t, 2)) {
			LOG_E("failedf to realloc")
			return NULL;
		}
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
	int value_int;
	for(int c = 0; c < t->cols_cnt - 1; c++) {
		size = t->columns[c + 1]->size;
		
		switch(t->columns[c + 1]->type) {
			case DT_INT:
			case DT_ID:
			case DT_INT32:
			case DT_INT16:
				value_int = atoi(datas[c]);
				memcpy(res + offset, &value_int, size);
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
 *	result - for table requests
 *
 */

// datas contain pointers to any types
char *line_new_with_id(table *t, char *datas[]) {
	char *res;
	MALLOC(res, t->line_size, return NULL, )
	
	size_t offset = 0;
	
	// adding rest from datas after conversion
	int value;
	for(int c = 0; c < t->cols_cnt; c++) {
		size_t size = t->columns[c]->size;
		
		switch(t->columns[c]->type) {
			case DT_INT:
			case DT_ID:
			case DT_INT16:
			case DT_INT32:
				value = atoi(datas[c]);
				memcpy(res + offset, &value, size);
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
	MALLOC(res, t->line_size, return NULL, )
	memcpy(res, line, t->line_size);
	
	return res;
}

char *line_get_data_ptr(table *t, char *line, int col_idx) {
	return line + col_get_offset(t, col_idx);
}

char *get_line_by_id(table *t, int id) {
	int id_idx = get_col_idx_by_name(t, "id");
	if(id_idx < 0) {
		LOG_E("ERROR: no such column:%s\n", "id"); 
		return NULL;
	}
	
	// index on id
	if(t->columns[0]->index != NULL) {
		#ifdef DB_DEBUG
			LOG_S("DEBUG: getting line from index")
		#endif
		

		char *found = hashtable_find(t->columns[0]->index, &id);
		
		if(!found) {
			LOG_E("ERROR: not found in index:[%d]\n", id)
		}
		else {
			return found;
		}
	}
	else {
		#ifdef DB_DEBUG
			LOG_S("DEBUG: no index")
		#endif
		
		// looking for id
		for(int i = 0; i < t->cnt; i++) {
			char *line = t->lines[i];
			
			if(*((int *) (line_get_data_ptr(t, line, id_idx))) == id) {
				return line;
			}
		}
	}
	
	LOG_E("ERROR: no such id:%d\n", id)
	
	return NULL;
}

result *table_get_line_by_id(table *t, int id) {
	// creating result table
	result *res = table_new(TT_RESULT, -1, -1, __func__);

	// adding columns
	for(int c = 0; c < t->cols_cnt; c++) {
		table_add_col(res, t->columns[c]->name, t->columns[c]->type, t->columns[c]->size, 0);
	}

	// getting line
	char *line = get_line_by_id(t, id);
	
	#ifdef DB_DEBUG
		LOG_S("DEBUG: line:[%d]\n", *(int *) line);
	#endif
	
	if(line) {
		// adding a copy into result
		char *line_result = line_copy(res, line);

		if(!line_result) {
			return NULL;
		}
		
		if(!table_add(res, line_result)) {
			LOG_E("failed to add line")
			return NULL;
		}
	}

	// even if empty
	return res;
}

result *table_get_data_by_id(table *t, int id, const char *colname) {
	// creating result table
	result *res = table_new(TT_RESULT, -1, -1, __func__);
	
	table_add_col(res, "id", DT_ID, 0, 0);
	
	// adding column
	int col_idx = get_col_idx_by_name(t, colname);
	if(col_idx < 0) {
		LOG_E("ERROR: no such column:%s\n", colname); 
		return res;
	}
	
	table_add_col(res, t->columns[col_idx]->name, t->columns[col_idx]->type, t->columns[col_idx]->size, 0);
	
	// getting line
	char *line = get_line_by_id(t, id); 
	if(line) {
		// creating the result line with (id, colname) and adding to result table
		table_add(res, line_new_with_id_correct_types(res, (void *[]) { &id, line_get_data_ptr(t, line, col_idx) }));
	}
	
	// even if empty
	return res;
}


/*
 *
 *	IO operations - save / load
 *
 */

table_type string_to_table_type(char *type) {
	if(!strcmp(type, "table")) {
		return TT_TABLE;
	}
	else if(!strcmp(type, "result")) {
		return TT_RESULT;
	}
	else {
		return TT_NONE;
	}
}

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
		table_add_col(res, cinfos->name, cinfos->type, cinfos->size, *(&(cinfos->size) + 1));
	}
	
	// read and add lines to table 
	for(int i = 0; i < tinfos->cnt; i++) {
		char *line;
		MALLOC(line, res->line_size + 1, return NULL, )
		fread(line, res->line_size + 1, 1, file);
		table_add(res, line);
	}
		
	return res;
}

const char *table_type_tos(table_type type) {
	switch(type) {
		case TT_NONE:
		default:
			return "none";
		case TT_TABLE:
			return "table";
		case TT_RESULT:
			return "result";
	}
}

// for dumping to a file
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
			case DT_INT:
			case DT_INT16:
			case DT_INT32:
			case DT_ID:
				sprintf(int_as_str, "%d", *((int *) line));
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

void line_fprintf_readable_separator(FILE *file, table *t, int col_width) {
	char sep[col_width + 2];
	memset(sep, '-', col_width + 1);
	sep[col_width + 1] = 0;
	for(int i = 0; i < t->cols_cnt; i++) {	
		fprintf(file, "%s", sep);
	}
	fprintf(file, "\n");
}

table *table_fprintf_readable(FILE *file, table *t, int col_width) {
	line_fprintf_readable_separator(file, t, col_width);

	fprintf(file, "%s[id=%d][name=%s][cols=%d][lines=%d]\n", table_type_tos(t->type), t->id, t->name, t->cols_cnt, t->cnt);

	line_fprintf_readable_separator(file, t, col_width);

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

	line_fprintf_readable_separator(file, t, col_width);

	for(int l = 0; l <  t->cnt; l++) {
		line_fprintf_readable(file, t, t->lines[l], col_width);		
	}

	line_fprintf_readable_separator(file, t, col_width);

	return t;
}

