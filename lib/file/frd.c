#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FRD_BUF_END 20
#define FRD_MESS_END 10

#define MESS_READ_ERROR -2
#define MESS_TOO_LONG -1
#define MESS_FINISHED 0
#define MESS_NOT_FINISHED 1
#define MESS_BUF_END 2
#define MESS_EOF 3

int frd_is_eom(char c) {
	if(c == '\0' || c == '\n' || c == EOF) {
		return 1;
	}
	return 0;
}

int frd_buf_next(char *message, int *m_pos, char *buffer, int len, int *b_pos) {
	while(*m_pos < FRD_MESS_END && *b_pos < len && *b_pos < FRD_BUF_END && !frd_is_eom(buffer[*b_pos])) {
		message[*m_pos] = buffer[*b_pos];
		(*m_pos)++;
		(*b_pos)++;
	}

	if(*b_pos >= len || *b_pos >= FRD_BUF_END) {
		if(*m_pos == 0) {
			return MESS_BUF_END;
		}
		return MESS_NOT_FINISHED;
	}

	if(*m_pos >= FRD_MESS_END) {
		fprintf(stderr, "message too long\n");
		return MESS_TOO_LONG;
	}

	/* eom has been read from b */
	message[*m_pos] = '\0';
	(*b_pos)++;

	return MESS_FINISHED;
}

int frd_handle(char *message) {
	printf("handle:%s:\n", message);

	return 1;
}

int frd_read(int fd, int (*handle)(char *)) {
	int ret;
	char message[FRD_MESS_END];
	char buffer[FRD_BUF_END];
	int b_pos, m_pos, n_read;

	n_read = read(fd, buffer, FRD_BUF_END);
	m_pos = 0;

	while(n_read > 0) {
		printf("read : %d\n", n_read);
		b_pos = 0;

		ret = frd_buf_next(message, &m_pos, buffer, n_read, &b_pos);

		while(ret == MESS_FINISHED) {
			handle(message);
			m_pos = 0;

			ret = frd_buf_next(message, &m_pos, buffer, n_read, &b_pos);
		}

		if(ret == MESS_NOT_FINISHED) {
			;
		}

		if(ret == MESS_TOO_LONG) {
			return MESS_TOO_LONG;
		}

		n_read = read(fd, buffer, FRD_BUF_END);
	}
	if(n_read == 0) {
		printf("No more read\n");
		return MESS_EOF;
	}
	else {
		perror("read error");
		return MESS_READ_ERROR;
	}
}

int frd_read_loop(int fd) {
	switch(frd_read(fd, frd_handle)) {
		case MESS_TOO_LONG:
			break;

		case MESS_EOF:
			sleep(2);
			frd_read_loop(fd);
			return 1;
			break;

		case MESS_READ_ERROR:
			break;
	}

	return 0;
}

int main(int argc, char **argv) {
	if(argc != 2) {
		printf("usage : %s filepath\n", argv[0]);
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);

	if(!fd) {
		perror("file open error");
		return -1;
	}

	frd_read_loop(fd);

	close(fd);

	return 0;
}


