#include <stdio.h>
#include <fcntl.h>
#include "../include/frd.h"

#define MESS_SIZE 10
#define BUF_SIZE 20

int frd_handle(char *message, int fd) {
	printf("handle:%d:%s:\n", fd, message);
	return 1;
}

int frd_read_switch(int fd) {
	char message[MESS_SIZE];
	char buffer[BUF_SIZE];
	size_t m_pos = 0;
	size_t b_pos = 0;
	int ret;

	while(1) {
		ret = frd_read(fd, frd_handle, &message[0], MESS_SIZE, &m_pos, &buffer[0], BUF_SIZE, &b_pos);

		switch(ret) {
			case FRD_TOO_LONG:
				fprintf(stderr, "TOO LONG\n");
				return 1;
				break;
			case FRD_READ_ERROR:
				fprintf(stderr, "READ_ERROR\n");
				return 1;
				break;

			case FRD_NOT_FINISHED:
			case FRD_EOB:
			case FRD_FINISHED:
				break;

			case FRD_EOF:
				return 0;

			default:
				return 1;
		}

		ret = frd_read(fd, frd_handle, &message[0], MESS_SIZE, &m_pos, &buffer[0], BUF_SIZE, &b_pos);
	}

	return 1;
}

int main(int argc, char **argv) {
	int ret;

	if(argc != 2) {
		printf("usage : %s filepath\n", argv[0]);
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(!fd) {
		perror("file open error");
		return -1;
	}

	ret = frd_read_switch(fd);

	close(fd);

	return ret;
}


