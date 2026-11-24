#include "arrayh.h"

#define CFG_FILE_MAX_LINE_LEN 1024
#define FILE_MAX_LINE_LEN 4096

#define STRING_EQUAL "="
#define STRING_SEMI ";"
#define CHAR_SEMI ';'
#define CHAR_EQUAL '='


char *file_readline_new_string(FILE *f);
size_t file_readline_to(FILE *f, char *dest, size_t size_of_dest);
char *file_readline(FILE *f, char *arrayh_dest);
char *file_readfile(FILE *f, size_t *len);


// config as an array of 2 elements (name, value)
int cfg_fprintf(char **a, void *stream, void *delim);

char ***fd_load_cfg(FILE *fd);
char ***fd_save_cfg(FILE *fd, char ***a);
char **fd_load_list(FILE *fd);
char **fd_save_list(FILE *fd, char **a);

// same as above, from path
char ***file_load_cfg(char *path);
char ***file_save_cfg(char *path, char ***a);
char **file_load_list(char *path);
char **file_save_list(char *path, char **a);

