#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "fb.h"
#include "fr.h"

/* str functions for array init */
FUNC_SF(str)


fb *fb_new(char *path, char *name, size_t init_size) {
	fb *res = (fb *)malloc(sizeof(*res));

	if(!res) {
		perror("fb_new : alloc failed");
		return NULL;
	}

	if(path) {
		strncpy(res->path, path, BUF_PATH_LEN);
	}
	else {
		res->path[0] = '\0';
	}

	if(name) {
		strncpy(res->name, name, BUF_NAME_LEN);
	}
	else {
		res->name[0] = '\0';
	}

	res->lines = array_new(str_sf(), init_size);
	if(!res->lines) {
		free(res);
		return NULL;
	}

	return res;
}

fb *fb_load(fb *buf) {
	FILE *f = fopen(buf->path, "r");
	if(!f) {
		perror("");
		return NULL;
	}
	int n_read;
	char *tmp;
	str *line;

	/* Add every new allocated lines into array */
	do {
		tmp = fr_readnextline(f, 128, &n_read);
		if(tmp && n_read > 0) {
			line = str_new(tmp, n_read, n_read);
			if(line) {
				buf->lines = array_append(buf->lines, line);
			}
		}

	} while(tmp && n_read > 0 && line && buf->lines);

	if(tmp) {
		free(tmp);
	}
	fclose(f);

	return buf;
}

void fb_free(fb *buf) {
	array_free(buf->lines, 1);
	free(buf);
}

void fb_print(fb *buf) {
	array_print(buf->lines);
}

fb *fb_save(fb *buf, char *newpath) {
	FILE *f;

	if(newpath) {
		f = fopen(newpath, "w");
	}
	else {
		f = fopen(buf->path, "w");
	}
	if(!f) {
		perror("fb_save : fopen error");
		return NULL;
	}

	for(size_t i = 0; i < buf->lines->n; i++) {
		str *l = buf->lines->data[i];
		size_t len = strlen(l->start);

		if(fwrite(l->start, sizeof(char), len, f) != len) {
			perror("fb_save : write error");
			return NULL;
		}
		if(fwrite("\n", sizeof(char), 1, f) != 1) {
			perror("fb_save : eol");
			return NULL;
		}
	}

	fclose(f);

	return buf;
}

