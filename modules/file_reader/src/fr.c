#include <string.h>
#include <unistd.h>
#include "fr.h"

#define FB_RC_FINISHED 3

char *fr_readnextline(FILE *f, size_t alloc, size_t realloc_coef, int *n_read) {
	char *res;
	char c;
	size_t i;

	res = malloc(sizeof(char) * (alloc + 1));
	if(!res) {
		perror("fr_readline : alloc failed");
		return NULL;
	}

	c = fgetc(f);
	for(i = 0; c != EOF && c !='\0'; i++) {
		if(i >= alloc) {
			alloc *= realloc_coef;
			res = realloc(res, sizeof(char) * (alloc + 1));
			if(!res) {
				perror("fr_readnextline : realloc failed");
				return NULL;
			}
		}
		if(c == '\n') {
			break;
		}
		res[i] = c;
		c = fgetc(f);
	}
	res[i] = '\0';

	if(c == '\n') {
		i++;
	}

	*n_read = i;

	return res;
}

char *fr_readfile(char *path) {
	char *res;
	long size;
	FILE *f;

	f = fopen(path, "rb");
	if(!f) {
		perror("fr_readfile : open error");
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);

	res = malloc(sizeof(char) * (size + 1));
	fread(res, size, 1, f);
	fclose(f);

	res[size] = 0;

	return res;
}

char *fr_writefile(char *path, char *string) {
	FILE *f = fopen(path, "wb");
	if(!f) {
		perror("fr_writefile : open error");
		return NULL;
	}
	if(fwrite(string, strlen(string), 1, f) != 1) {
		return NULL;
	}
	fclose(f);

	return string;
}

char _fr_EOL[] = { '\0', '\n', EOF };

int _fr_char_is_eol(char c) {
	for(size_t i = 0; i < sizeof(_fr_EOL) / sizeof(char); i++) {
		if(c == _fr_EOL[i]) {
			return 1;
		}
	}
	return 0;
}

int _fr_part_next(char *line, size_t lsize, size_t *llen, \
					char *buffer, size_t bsize, size_t *bpos, \
					size_t n_read) {
	while(*llen < lsize && \
			*bpos < n_read && *bpos < bsize && \
					!_fr_char_is_eol(buffer[*bpos])) {
		line[*llen] = buffer[*bpos];
		(*llen)++;
		(*bpos)++;
	}

	if(*bpos >= n_read || *bpos >= bsize) {
		if(*llen == 0) {
			*bpos = 0;
			return FB_RC_EOB;
		}
		return FB_RC_NOT_FINISHED;
	}

	if(*llen >= lsize) {
		(*llen)--;
		(*bpos)--;
		line[*llen] = '\0';
		return FB_RC_TOO_LONG;
	}

	/* eom has been read from b */
	line[*llen] = '\0';
	(*bpos)++;

	return FB_RC_FINISHED;
}

int fr_part_read(int fd, void (*handler)(int, char *, int), \
				char *line, size_t lsize, size_t *llen, \
				char *buffer, size_t bsize, size_t *bpos) {
	int ret;
	size_t n_read;

	memset(buffer, 0, bsize);
	n_read = read(fd, buffer, bsize);
	*bpos = 0;

	if(n_read > 0) {
		ret = _fr_part_next(line, lsize, llen, buffer, bsize, bpos, n_read);

		while(ret == FB_RC_FINISHED) {
			handler(fd, line, *llen);
			*llen = 0;

			ret = _fr_part_next(line, lsize, llen, buffer, bsize, bpos, n_read);
		}

		return ret;
	}
	else if(n_read == 0) {
		return FB_RC_EOF;
	}
	else {
		perror(strerror(n_read));
		return FB_RC_READ_ERROR;
	}
}


