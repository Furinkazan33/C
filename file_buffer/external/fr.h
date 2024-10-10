#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *fr_readnextline(FILE *f, size_t alloc, size_t realloc_coef, int *n_read);
char *fr_readfile(char *path);


