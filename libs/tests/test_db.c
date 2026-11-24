#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include "db.h"
#include "macros.h"



int main() {
	table *t = table_new(TT_TABLE, -1, -1, "person"); if(!t) return 1;
	
	if(!table_add_col(t, "id", DT_ID, 0, 20)) return 1;
	if(!table_add_col(t, "firstname", DT_TEXT, 16, 0)) return 1;
	if(!table_add_col(t, "lastname", DT_TEXT, 64, 0)) return 1;
	if(!table_add_col(t, "age", DT_INT32, 0, 0)) return 1;
	if(!table_add_col(t, "email", DT_TEXT, 256, 0)) return 1;
	if(!table_add_col(t, "group", DT_CHAR, 0, 20)) return 1;

	char *l;
	for(int i = 0; i < 40; i++) {
		char c = ' ' + i;
		char age[10]; sprintf(age, "%d", i + 1);

		l = line_new(t, (char *[]) { "Toto", "Tata", age, "mv@free.fr", &c });
		if(!l) return 1;
		if(!table_add_line(t, l)) return 1;
	}

	//hashtable_log_as_int(t->columns[0]->index, true);
	//hashtable_log_as_int(t->columns[5]->index, true);

	FILE *fout = fopen("table.out", "w"); if(!fout) { LOG_E("failed to open file\n"); return 1; }
	table_write(fout, t);
	fclose(fout);
	free(t);


	puts("\n");
	FILE *fin = fopen("table.out", "r"); if(!fin) { LOG_E("failed to open file\n"); return 1; }
	t = table_read(fin); if(!t) { LOG_E("failed to load table\n"); return 1; }
	fclose(fin);
	table_fprintf_readable(stdout, t, 15);

	
	puts("\n");
	LOG_S("INFO : table_get_line_by_id\n"); 
	result *rline = table_get_line_by_id(t, 6, true); if(!rline) return 1;
	table_fprintf_readable(stdout, rline, 15);
	table_free(rline, true);
	
	// changing age with line ptr
	l = table_get_line_ptr_by_id(t, 6); if(!l) return 1;
	int *age = (int *) line_get_data_ptr(t, l, 3);
	*age = 12;
	rline = table_get_line_by_id(t, 6, true); if(!rline) return 1;
	table_fprintf_readable(stdout, rline, 15);
	table_free(rline, true);
	
	// rechanging age with data ptr
	age = (int *) table_get_data_ptr_by_id(t, 6, "age"); if(!age) { return 1; }
	*age = 32;
	rline = table_get_line_by_id(t, 6, true); if(!rline) return 1;
	table_fprintf_readable(stdout, rline, 15);
	table_free(rline, true);
	
	
	puts("\n");
	LOG_S("INFO : table_get_data_by_value_char(t, \"group\", '%%', \"age\")\n"); 
	result *rage = table_get_data_by_value_char(t, "group", '%', "age"); if(!rage) return 1;
	table_fprintf_readable(stdout, rage, 15);
	table_free(rage, true);
	
	puts("\n");
	LOG_S("INFO : table_get_data_by_value_int(t, \"age\", 32, NULL)\n"); 
	result *rids = table_get_data_by_value_int32(t, "age", 32, NULL); if(!rids) return 1;
	table_fprintf_readable(stdout, rids, 15);
	table_free(rids, true);
	
	puts("\n");
	LOG_S("INFO : table_get_data_by_value_int(t, \"age\", 32, \"email\")\n"); 
	result *remails = table_get_data_by_value_int32(t, "age", 32, "email"); if(!remails) return 1;
	table_fprintf_readable(stdout, remails, 15);
	table_free(remails, true);
	
	
	puts("\n");
	LOG_S("INFO : table_get_line_by_value_int(t, \"age\", 32)\n"); 
	result *age32 = table_get_line_by_value_int32(t, "age", 32, true); if(!age32) return 1;
	table_fprintf_readable(stdout, age32, 15);
	table_free(age32, true);


	table_free(t, true);



	return 0;
}
