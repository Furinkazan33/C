#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUF_LINES_ALLOC 256
#define BUF_LINE_ALLOC 128
#define BUF_LINE_CONTENT_ALLOC 128
#define BUF_NAME_LEN 32
#define BUF_PATH_LEN 256

typedef struct line {
	int type;
	size_t size;
	char *content;
} line;

typedef struct txt_buf {
	char name[BUF_NAME_LEN];
	char filepath[BUF_PATH_LEN];
	size_t size;
	size_t n;
	line **lines;
} txt_buf;

typedef txt_buf tb;

line **tb_alloc_lines(tb *buf) {
	line **new_lines;

	/* first alloc */
	if(!buf->lines) {
		buf->size = BUF_LINES_ALLOC;
		buf->lines = (line **)malloc(sizeof(line *) * buf->size);

		if(!buf->lines) {
			perror("");
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
			perror("");
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
			perror("");
			return NULL;
		}
		dest->size = BUF_LINE_CONTENT_ALLOC;
		dest->content = (char *)malloc(sizeof(char) * (dest->size + 1));
		if(!dest->content) {
			perror("");
			return NULL;
		}
	}
	while(min >= dest->size) {
		dest->size *= 2;
		new_c = (char *)realloc(dest->content, sizeof(char) * (dest->size + 1));
		if(!new_c) {
			perror("");
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
	for(i = 0; c != '\n' && c != EOF; i++) {
		dest = tb_alloc_line(dest, i + 1);
		if(!dest) {
			return NULL;
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

tb *tb_new(char *filepath, char *name) {
	tb *res = (tb *)malloc(sizeof(*res));

	if(!res) {
		perror("");
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
	static line *buffer;

	f = fopen(buf->filepath, "r");
	if(!f) {
		perror("");
		return 0;
	}

	buffer = tb_readline(buffer, f, &n_read);

	for(int i = 0; n_read > 0;  i++) {
		if(!buffer) {
			return 0;
		}
		buf->lines[i] = tb_alloc_line(buf->lines[i], n_read); 
		if(!buf->lines[i]) {
			return 0;
		}
		strncpy(buf->lines[i]->content, buffer->content, n_read);
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
	if(!f) {
		perror("");
		return 0;
	}

	for(size_t i = 0; i < buf->n; i++) {
		printf("%s\n", buf->lines[i]->content);

		if(fwrite(buf->lines[i]->content, strlen(buf->lines[i]->content) + 1, 1, f) != 1) {
			perror("");
			return 0;
		}
	}

	fclose(f);

	return 1;
}

void tb_free(tb *buf) {
	for(size_t i = 0; i < buf->n; i++) {
		free(buf->lines[i]->content);
		free(buf->lines[i]);
	}
	free(buf);
}


