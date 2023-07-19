#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "../include/frd.h"


enum serv_t { START = 0, STOP, RESTART };
char *serv_m[] = { [START] = "SERV_START", [STOP] = "SERV_STOP", [RESTART] = "SERV_RESTART", };
static int serv_s[3];

int ff_handler(char *message, int fd) {
	fprintf(stdout, "[mess_treat][%d][%s]\n", fd, message);

	if(strcmp(message, serv_m[STOP]) == 0) {
		serv_s[STOP] = 1;
	}
	else if(strcmp(message, serv_m[RESTART]) == 0) {
		serv_s[RESTART] = 1;
	}
	else if(strcmp(message, serv_m[START]) == 0) {
		serv_s[START] = 1;
	}
	else {
		fprintf(stderr, "unrecognize message\n");
		return 0;
	}

	return 1;
}

int main(void) {
	int ff_fd;
	size_t ff_s = 1024;
	char ff_m[ff_s];
	char ff_b[ff_s];
	size_t ff_mpos = 0;
	size_t ff_bpos = 0;

	ff_fd = open("./dev/fifo_rd", O_RDONLY);
	if(!ff_fd) {
		perror("");
		return 1;
	}

	switch(frd_read(ff_fd, ff_handler, &ff_m[0], ff_s, &ff_mpos, &ff_b[0], ff_s, &ff_bpos)) {
		case FRD_READ_ERROR:
		printf("FRD_READ_ERROR\n");
		break;

		case FRD_TOO_LONG:
		printf("FRD_TOO_LONG\n");
		break;
	}

	close(ff_fd);

	return 0;
}

