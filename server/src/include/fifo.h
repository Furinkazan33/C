#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define FIFO_BUF_LEN 64
#define CHAR_EOM ';'

static int fifo_read(int fd, char *message, int max_len, int (*treat)(char *)) {
	int j = strlen(message);
	char buffer[FIFO_BUF_LEN];
	int n_read = read(fd, buffer, FIFO_BUF_LEN);

	for(int i = 0; i < n_read; i++) {
		if(j >= max_len) {
			fprintf(stderr, "[FIFO_RD] message too long:%s:\n", message);
			memset(message, 0, max_len);
			return 1;
		}

		/* End of message */
		if(buffer[i] == CHAR_EOM) {
			message[j] = '\0';

			if(treat(message) != 0) {
				memset(message, 0, max_len);
				return 1;
			}
			memset(message, 0, max_len);
		}
		else {
			if(buffer[i] != '\n' && buffer[i] != '\r') {
				message[j++] = buffer[i];
			}
		}
	}
	return 0;
}


