#include <stdio.h>
#include <unistd.h>

#define FRD_READ_ERROR -2
#define FRD_TOO_LONG -1
#define FRD_FINISHED 0
#define FRD_NOT_FINISHED 1
#define FRD_EOB 2
#define FRD_EOF 3


int frd_read(int fd, int (*handle)(char *, int), char *message, size_t mess_size, size_t *m_pos, char *buffer, size_t buf_size, size_t *b_pos);


