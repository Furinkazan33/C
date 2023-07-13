#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../include/fifo.h"

int mess_treat(char *message) {
	//fprintf(stderr, "[FIFO_RD]:%s:\n", message);
	//return 1;

	fprintf(stdout, "[FIFO_RD] message ok:%s:\n", message);

	return 0;
}

int main(void) {
	int fd_rd;

	fd_rd = open("./dev/fifo_rd", O_RDONLY);
	if(!fd_rd) {
		perror("");
		return 1;
	}

	while(1) {
		fifo_read(fd_rd, ';', mess_treat);

		sleep(1);
	}

	close(fd_rd);

	return 0;
}

