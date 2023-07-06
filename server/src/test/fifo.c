#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "fifo.h"

#define BUF_LEN 64
#define MESS_LEN 20
#define CHAR_EOM ';'

int mess_treat(char *message) {
	//fprintf(stderr, "[FIFO_RD]:%s:\n", message);
	//return 1;

	fprintf(stdout, "[FIFO_RD] message ok:%s:\n", message);

	return 0;
}

int main(void) {
	int fd_rd;
	static char message[MESS_LEN];

	fd_rd = open("./dev/fifo_rd", O_RDONLY);
	if(!fd_rd) {
		perror("");
		return 1;
	}

	while(1) {
		fifo_read(fd_rd, message, MESS_LEN, mess_treat);

		sleep(1);
	}

	return 0;
}

