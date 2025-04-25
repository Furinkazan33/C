#include <stdlib.h>
#include <stdio.h>
#include "fr.h"



int main() {
	size_t len;
	FILE *fd = fopen("fr.c", "r");
	char dest[128] = {0};

	while(*fr_readline(fd, dest, &len)) {
		printf("%s", dest);
	}

	fclose(fd);
}


