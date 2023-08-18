#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "book.h"

int book_cmp(void *b1, void *b2) {
	return strcmp(((book *)b1)->name, ((book *)b2)->name);
}
void book_free(void *b) {
	array_free(((book *)b)->sheets, 1);
	free(b);
}
void book_print(void *bo) {
	array_print(((book *)bo)->sheets);
}

int sheet_cmp(void *c1, void *c2) {
	return strcmp(((sheet *)c1)->name, ((sheet *)c2)->name);
}
void sheet_free(void *s) {
	array_free(((sheet *)s)->cols, 1);
	free(s);
}
void sheet_print(void *s) {
	array_print(((sheet *)s)->cols);
}

int col_cmp(void *c1, void *c2) {
	return ((col *)c1)->n - ((col *)c2)->n;
}
void col_free(void *c) {
	array_free(((col *)c)->cells, 1);
	free(c);
}
void col_print(void *c) {
	array_print(((col *)c)->cells);
}

int cell_cmp(void *c1, void *c2) {
	return strcmp(((cell *)c1)->value, ((cell *)c2)->value);
}
void cell_free(void *c) {
	free(((cell *)c)->value);
	free(c);
}
void cell_print(void *c) {
	printf("%s\n", ((cell *)c)->value);
}




book *book_new(char *path, char *name, size_t sheets) {
	book *res = malloc(sizeof(*res));
	if(!res) {
		perror("book_new : alloc failed");
		return NULL;
	}

	if(path) {
		strncpy(res->path, path, MAX_PATH_LEN);
	}
	else {
		res->path[0] = '\0';
	}

	if(name) {
		strncpy(res->name, name, MAX_NAME_LEN);
	}
	else {
		res->name[0] = '\0';
	}

	res->sheets = array_new(sheets);
	//sheet_cmp, sheet_free, sheet_print);
	if(!res->sheets) {
		book_free(res);
		return NULL;
	}

	return res;
}

sheet *sheet_new(char *name, size_t cols) {
	sheet *res = malloc(sizeof(*res));
	if(!res) {
		perror("");
		return NULL;
	}

	strncpy(res->name, name, MAX_NAME_LEN);

	res->cols = array_new(cols);
	//, col_cmp, col_free, col_print);
	if(!res->cols) {
		sheet_free(res);
		return NULL;
	}

	return res;
}

col *col_new(size_t cells, size_t n) {
	col *res = malloc(sizeof(*res));
	if(!res) {
		perror("");
		return NULL;
	}

	res->cells = array_new(cells);
	//, cell_cmp, cell_free, cell_print);
	if(!res->cells) {
		col_free(res);
		return NULL;
	}
	res->n = n;

	return res;
}

cell *cell_new(size_t len, size_t n) {
	assert(len < MAX_VALUE_LEN);

	cell *res = malloc(sizeof(*res));
	if(!res) {
		perror("");
		return NULL;
	}

	res->value = malloc(sizeof(char) * (len + 1));
	if(!res->value) {
		cell_free(res);
		return NULL;
	}
	res->size = len;
	res->n = n;

	return res;
}

cell *cell_set_value(cell *ce, char *value, size_t len) {
	assert(len < MAX_VALUE_LEN);

	if(len > ce->size) {
		ce->value = realloc(ce->value, ce->size * 2);
		if(!ce->value) {
			perror("");
			return NULL;
		}
		ce->size *= 2;
		strcpy(ce->value, value);
	}
	return ce;
}

