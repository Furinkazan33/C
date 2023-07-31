#include <stdlib.h>
#include "array.h"
#include "str.h"

/*
 * File Buffer (fb) for reading and writing files :
 * - load/save file to/from buffer
 * - operations on lines
 */

#define BUF_NAME_LEN 32
#define BUF_PATH_LEN 256

typedef struct file_buffer {
	char name[BUF_NAME_LEN];
	char path[BUF_PATH_LEN];
	array *lines;
} file_buffer;

typedef file_buffer fb;

/* Allocations */
fb *fb_new(char *path, char *name, size_t init_size);

/* reading filepath and adding lines to array */
fb *fb_load(fb *buf);

/* if newpath is null, save to current filepath */
fb *fb_save(fb *buf, char *newpath);

void fb_free(fb *buf);
void fb_print(fb *buf);
