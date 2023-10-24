#include <stdlib.h>
#include <stdio.h>

typedef enum c_log_stat_type {
	CLT_NONE = 0,
	CLT_VALUE,
	CLT_VAR,
	CLT_FUNCALL,
} c_log_stat_type;

typedef struct c_log_stat {
	c_log_stat_type type;
	void *data;
} c_log_stat;





int main(void) {

	char *log[] = { "a", "&&", "(", "0", "||", "12", ")" }



	if("(") {
		// read logical expression
	}

	return 0;
}
