#include "XXXX.h"
#include <string.h>


char *gen(app *a) {
	return gen_type(a, 0);
}

char *gen_type(app *a, size_t tabs) {
	if(!strcmp(a->type, "TABLE_W_HEADERS") || !strcmp(a->type, "TABLE")) {
		return gen_table(a, tabs);
	}
	else {
		return gen_all(a, tabs);
	}
	
	return NULL;
}


char *gen_all(app *a, size_t tabs) {
	char *res;
	size_t alloc = 512;
	MALLOC(res, alloc, LOG_E("malloc failed"); return NULL, {
		TAB(res, tabs)
	
		strcat(res, "<"); strcat(res, a->type); strcat(res, ">"); strcat(res, "\n");

		if(a->children) {
			ARRAY_DO(a->children, idx, app *, c, strcat(res, gen_type(c, tabs + 1)))
		}
		
		TAB(res, tabs)

		strcat(res, "</"); strcat(res, a->type); strcat(res, ">"); strcat(res, "\n");
		
		return res;
	});
}


char *gen_table(app *a, size_t tabs) {
	char *res;
	size_t alloc = 512;
	MALLOC(res, alloc, LOG_E("malloc failed"); return NULL, {
		TAB(res, tabs)
	
		strcat(res, "<table>\n");
		
		char *(*data)[] = a->src->data;
		
		
		for(size_t l = 0; (*data)[l]; l++) {
			
			TAB(res, tabs)
			
			if(l == 0 && !strcmp(a->type, "TABLE_W_HEADERS")) {
				strcat(res, "\t<th>\n");
			}
			else {
				strcat(res, "\t<tr>\n");
			}
			
			
			char *(*line)[] = (char *(*)[]) (*data)[l];
			
			for(size_t c = 0; (*line)[c]; c++) {
				
				TAB(res, tabs)
				
				strcat(res, "\t\t<td>");
				strcat(res, (char *) (*line)[c]);
				strcat(res, "</td>\n");
			}
			
			TAB(res, tabs)
			
			if(l == 0 && !strcmp(a->type, "TABLE_W_HEADERS")) {
				strcat(res, "\t</th>\n");
			}
			else {
				strcat(res, "\t</tr>\n");
			}
		}
		
		TAB(res, tabs)
		
		strcat(res, "</table>\n");
		
		return res;
	});
}

