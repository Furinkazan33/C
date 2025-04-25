#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fifo.h"
#include "str.h"

char *extract_message(char *dest, char *src, char eom) {
	char *delim = strchr(src, eom);
	if(delim) {
		int i = delim - src;
		strncpy(dest, src, i + 1);
		dest[i + 1] = 0;
		str_shift_left(src, i + 1);
	}
	else {
		dest[0] = 0;
	}
	return dest;
}

size_t fifo_read(int fd, char *buffer) {
	size_t rest = strlen(buffer);
	size_t n = read(fd, buffer + rest, sizeof(char) * FIFO_READ);

	// setting end of buffer
	if(buffer[rest + n - 1 ] == '\n') {
		buffer[rest + n - 1] = 0;
	}
	else {
		buffer[rest + n] = 0;
	}

	return n;
}



