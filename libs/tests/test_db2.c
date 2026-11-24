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


	FILE *fout = fopen("table_schema.out", "w"); if(!fout) { LOG_E("failed to open file\n"); return 1; }
	table_fprintf_readable_schema(fout, t);
	fclose(fout);
	table_free(t, true);


	puts("\n");
	FILE *fin = fopen("table_schema.out", "r"); if(!fin) { LOG_E("failed to open file\n"); return 1; }
	t = table_new_from_schema(fin); if(!t) { LOG_E("failed to create table from schema\n"); return 1; }
	fclose(fin);
	table_fprintf_readable(stdout, t, 15);
	table_free(t, true);

	return 0;
}
