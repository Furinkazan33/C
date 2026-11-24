#include <stddef.h>
#include <stdbool.h>



typedef enum { DT_NONE = 0, DT_DELETED, DT_CREATED, DT_TIME, } diff_type;

const char *dt_tos(diff_type t);


typedef struct file {
	char *path;
	char *content;
	int mtime;
	char pad[4];
} file;

typedef struct filediff {
	diff_type type;
	bool treated;
	char pad[3];
	file *f;
} filediff;


filediff *filediff_new(char *path, char *content, int mtime, diff_type type);
void filediff_free(filediff *diff);
filediff **files_to_filediff(char **files, size_t len);
// compare first list against second
filediff **fds_compare(filediff **fds1, size_t len1, filediff **fds2, size_t len2, size_t *len);

//TODO
filediff **fds_fuse(filediff **fds1, size_t len1, filediff **fds2, size_t len2, size_t *len);

