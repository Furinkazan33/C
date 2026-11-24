#include <stdlib.h>
#include <stdio.h>
#include "file.h"
#include "macros.h"


#define SIZE 49


int main() {
	size_t len;
	FILE *fd;
	
	FOPEN(fd, "tests/test_file.c", "r", return 1, );
	char dest[SIZE];
	len = file_readline_to(fd, dest, SIZE);
	
	while(len > 0) {
		if(len >= SIZE - 1) {
			LOG_E("truncated line: [%s]\n", dest);
		}
		else {
			LOG_S("%s", dest);
		}
		len = file_readline_to(fd, dest, SIZE);
	}
	fclose(fd);


	/* use of only 1 array */
	FOPEN(fd, "tests/test_file.c", "r", return 1, );
	char *line;
	ARRAYH_NEW(line, 20, return 1, );
	while(1) {
		if(!file_readline(fd, line)) return 1;
		if(ARRAYH_CNT(line) == 1) { break; }
		LOG_S("%s", line);
	}
	ARRAYH_FREE(line);
	fclose(fd);
	
	
	FOPEN(fd, "tests/test_file.c", "r", return 1, );
	char *file = file_readfile(fd, &len); if(!file) return 1;
	LOG_S("\n%s", file);
	free(file);
	fclose(fd);

	return 0;	
}

