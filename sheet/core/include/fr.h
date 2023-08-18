#include <stdlib.h>
#include <stdio.h>

/*
 * File Reader (fr) for reading and writing files.
 *
 */

char *fr_readnextline(FILE *f, size_t alloc, size_t realloc_coef, int *n_read);
char *fr_readfile(char *path);
char *fr_writefile(char *path, char *string);


/* Partial reads are meant to be used for files like fifos and sockets
 * when the whole content is not yet available.
 * A buffer store the last read and the function tries to get a complete
 * line/message from it, then call the handler function on it.
 * Usefull to get complete messages easily.
 */

/* Return codes for partial reads */
#define FB_RC_READ_ERROR -2		/* read error on file */
#define FB_RC_TOO_LONG -1		/* when line_length > line_size */
#define FB_RC_NOT_FINISHED 1 	/* current line in buffer not finished */
#define FB_RC_EOB 2				/* end of current buffer (EOF not yet read) */
#define FB_RC_EOF 0				/* end of file */

/* This function must be called in a loop until FB_RC_EOF (end of file reached).
 * line is complete when a EOL char is read ('\0', '\n' or EOF).
 * line_size : length of allocated line
 * buf_size : size of allocated buffer
 * line_length : current length of line
 * buffer_position : current position in buffer
 * handler : gets current fd, line with its length as parameters
 * */
int fr_part_read(int fd, void (*handler)(int, char *, int), \
				char *line, size_t line_size, size_t *line_length, \
				char *buffer, size_t buf_size, size_t *buffer_position);


