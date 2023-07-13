#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

/* For memory allocations */
#define BUF_LINES_ALLOC 256
#define BUF_LINE_ALLOC 128
#define BUF_LINE_CONTENT_ALLOC 128
#define BUF_NAME_LEN 32
#define BUF_PATH_LEN 256
#define BUF_LINE_TAGS 20

/* Available line marks  */
#define TB_IGN   0x01
#define TB_EMPT  0x02
#define TB_COM   0x04
#define TB_DUP   0x08
#define TB_ERR   0x16
#define TB_NEW   0x32
#define TB_INFO  0x64
#define TB_WARN  0x128
#define TB_LOCK  0x256
#define TB_WRITE 0x512
#define TB_DEL   0x1024
#define TB_F11   0x2048
#define TB_F12   0x4096
#define TB_F13   0x8192
#define TB_F14   0x16384
#define TB_F15   0x32768


typedef struct line {
	uint16_t marks;
	char *content;
	size_t size;
} line;

typedef struct txt_buf {
	char name[BUF_NAME_LEN];
	char filepath[BUF_PATH_LEN];
	size_t size;
	size_t n;
	line **lines;
} txt_buf;

typedef txt_buf tb;

void tb_mark(line *l, int mark) {
	l->marks &= mark;
}

void tb_free(tb *buf) {
	for(size_t i = 0; i < buf->n; i++) {
		free(buf->lines[i]->content);
		free(buf->lines[i]);
	}
	free(buf);
}

void tb_free_line(line *l) {
	free(l->content);
}

line **tb_alloc_lines(tb *buf) {
	line **new_lines;

	/* first alloc */
	if(!buf->lines) {
		buf->size = BUF_LINES_ALLOC;
		buf->lines = (line **)malloc(sizeof(line *) * buf->size);

		if(!buf->lines) {
			perror("tb_alloc_lines : first alloc failed");
			return NULL;
		}
		buf->n = 0;

		return buf->lines;
	}

	/* realloc */
	if(buf->n >= buf->size) {
		buf->size *= 2;
		new_lines = (line **)realloc(buf->lines, sizeof(line *) * buf->size);

		if(!new_lines) {
			perror("tb_alloc_lines : realloc failed");
			return NULL;
		}
		buf->lines = new_lines;
		return buf->lines;
	}
	return buf->lines;
}

line *tb_alloc_line(line *dest, size_t min) {
	char *new_c;
	if(!dest) {
		dest = (line *)malloc(sizeof(line));
		if(!dest) {
			perror("tb_alloc_line : line alloc failed");
			return NULL;
		}
		dest->size = BUF_LINE_CONTENT_ALLOC;
		dest->content = (char *)malloc(sizeof(char) * (dest->size + 1));
		if(!dest->content) {
			perror("tb_alloc_line : content alloc failed");
			return NULL;
		}
	}
	while(min >= dest->size) {
		dest->size *= 2;
		new_c = (char *)realloc(dest->content, sizeof(char) * (dest->size + 1));
		if(!new_c) {
			perror("tb_alloc_line : content realloc failed");
			return NULL;
		}
		dest->content = new_c;
	}

	return dest;
}

line *tb_readline(line *dest, FILE *f, int *n_read) {
	char c;
	int i;

	c = fgetc(f);
	for(i = 0; c != EOF; i++) {
		dest = tb_alloc_line(dest, i + 1);
		if(!dest) {
			return NULL;
		}
		if(c == '\n') {
			break;
		}
		dest->content[i] = c;
		c = fgetc(f);
	}
	dest->content[i] = '\0';

	if(c == '\n') {
		i++;
	}

	*n_read = i;

	return dest;
}

tb *tb_new(const char *filepath, const char *name) {
	tb *res = (tb *)malloc(sizeof(*res));

	if(!res) {
		perror("tb_new : alloc failed");
		return NULL;
	}

	if(!tb_alloc_lines(res)) {
		return NULL;
	}

	if(filepath) {
		strncpy(res->filepath, filepath, BUF_PATH_LEN);
	}

	if(name) {
		strncpy(res->name, name, BUF_NAME_LEN);
	}

	return res;
}


int tb_load(tb *buf) {
	FILE *f;
	int n_read;
	line *buffer = NULL;

	f = fopen(buf->filepath, "r");
	if(!f) {
		perror("tb_load : alloc failed");
		return 0;
	}

	buffer = tb_readline(buffer, f, &n_read);

	for(int i = 0; n_read > 0;  i++) {
		if(buffer == NULL) {
			return 0;
		}

		buf->lines[i] = tb_alloc_line(buf->lines[i], n_read);
		if(buf->lines[i] == NULL) {
			return 0;
		}

		if(buffer->content[0] == '\0') {
			buf->lines[i]->content[0] = '\0';
		}
		else {
			strncpy(buf->lines[i]->content, buffer->content, n_read);
		}
		buf->n++;

		if(!tb_alloc_lines(buf)) {
			return 0;
		}
		buffer = tb_readline(buffer, f, &n_read);
	}

	free(buffer);

	fclose(f);

	return 1;
}

int tb_save(tb *buf) {
	FILE *f = fopen(buf->filepath, "w");
	line *buffer = NULL;
	size_t len;
	if(!f) {
		perror("tb_save : fopen error");
		return 0;
	}

	for(size_t i = 0; i < buf->n; i++) {
		printf("%s\n", buf->lines[i]->content);

		len = strlen(buf->lines[i]->content) + 1;
		buffer = tb_alloc_line(buffer, len);

		if(!buffer) {
			perror("tb_save : buffer alloc failed");
			return 0;
		}
		strcpy(buffer->content, buf->lines[i]->content);
		buffer->content[len - 1] = '\n';

		if(fwrite(buffer->content, sizeof(char), len, f) != len) {
			perror("tb_save write error");
			return 0;
		}
	}

	fclose(f);

	tb_free_line(buffer);

	return 1;
}

