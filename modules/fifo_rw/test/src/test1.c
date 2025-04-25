#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "macros.h"
#include "fifo.h"


int test_1() {
	char *myfifo = "/home/mathieu/C/modules/fifo_read/test/ici";
	mkfifo(myfifo, 0666);
	int fd = open(myfifo, O_RDONLY | O_NONBLOCK);
	char buffer[FIFO_BUF_LEN] = { 0 };
	char message[FIFO_BUF_LEN];

	while(1) {
		sleep(0.5);
		fifo_read(fd, buffer);
		if(*extract_message(message, buffer, ';')) {
			printf("message:[%s]\n", message);

			if(!strcmp(message, "exit;")) { break; }
		}
	}

	close(fd);

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 1) {
		fprintf(stderr, "usage : %s\n", argv[0]);
		return 1;
	}

	TEST_INIT(argv[0]);
	TEST(1);
	TEST_RESULTS();
}

