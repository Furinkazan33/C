#include <stdio.h>
#include <unistd.h>
#include "frd.h"
#include <string.h>

static char EOM[] = { '\0', '\n', EOF };

static int frd_is_eom(char c) {
	for(size_t i = 0; i < sizeof(EOM) / sizeof(char); i++) {
		if(c == EOM[i]) {
			return 1;
		}
	}
	return 0;
}

static int frd_buf_next(char *message, size_t mess_size, size_t *m_pos, char *buffer, size_t buf_size, size_t *b_pos, size_t n_read) {
	while(*m_pos < mess_size && *b_pos < n_read && *b_pos < buf_size && !frd_is_eom(buffer[*b_pos])) {
		message[*m_pos] = buffer[*b_pos];
		(*m_pos)++;
		(*b_pos)++;
	}

	if(*b_pos >= n_read || *b_pos >= buf_size) {
		if(*m_pos == 0) {
			*b_pos = 0;
			return FRD_EOB;
		}
		return FRD_NOT_FINISHED;
	}

	if(*m_pos >= mess_size) {
		(*m_pos)--;
		(*b_pos)--;
		message[*m_pos] = '\0';
		return FRD_TOO_LONG;
	}

	/* eom has been read from b */
	message[*m_pos] = '\0';
	(*b_pos)++;

	return FRD_FINISHED;
}

int frd_read(int fd, int (*handle)(char *, int), char *message, size_t mess_size, size_t *m_pos, char *buffer, size_t buf_size, size_t *b_pos) {
	int ret;
	size_t n_read;

	memset(buffer, 0, buf_size);
	n_read = read(fd, buffer, buf_size);
	*b_pos = 0;

	if(n_read > 0) {
		ret = frd_buf_next(message, mess_size, m_pos, buffer, buf_size, b_pos, n_read);

		while(ret == FRD_FINISHED) {
			handle(message, fd);
			*m_pos = 0;

			ret = frd_buf_next(message, mess_size, m_pos, buffer, buf_size, b_pos, n_read);
		}

		return ret;
	}
	else if(n_read == 0) {
		return FRD_EOF;
	}
	else {
		perror(strerror(n_read));
		return FRD_READ_ERROR;
	}
}


