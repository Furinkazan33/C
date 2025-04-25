#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#define FIFO_BUF_LEN 1024
#define FIFO_READ 512

char *extract_message(char *dest, char *src, char eom);
size_t fifo_read(int fd, char *buffer);


