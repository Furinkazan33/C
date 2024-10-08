#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "fb.h"
#include "fr.h"


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

	res->lines = arrayptr_new(init_size);
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
	while(1) {
		tmp = fr_readnextline(f, 64, 2, &n_read);
		if(!tmp) {
			perror("fb_load : error on read");
			return NULL;
		}
		if(n_read > 0) {
			line = str_new(tmp);
			if(!line) {
				perror("");
				return NULL;
			}
			if(!arrayptr_append(buf->lines, line)) {
				perror("");
				return NULL;
			}
		}
		else {
			break;
		}
	}

	free(tmp);
	fclose(f);

	return buf;
}

void fb_free(fb *buf) {
	ARRAYPTR_FREE(buf->lines);
	free(buf);
}

void fb_write(FILE *file, fb *buf) {
	FOR(i, 0, buf->lines->count, fprintf(file, buf->lines->items[i]););
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

	for(size_t i = 0; i < buf->lines->count; i++) {
		str *l = buf->lines->items[i];
		size_t len = strlen(l->data);

		if(fwrite(l->data, sizeof(char), len, f) != len) {
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

