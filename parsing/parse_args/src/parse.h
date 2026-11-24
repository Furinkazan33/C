#include <stdbool.h>
#include <stdio.h>

char *readline(FILE *f, char *dest, size_t *len);
char *readfile(const char *filename);

typedef enum {
	STRING = 0,
	STRING_LIT,
	INTEGER,
	DOUBLE,
	CHAR
} value_type;


value_type get_type(char *s, size_t len);
const char *type_to_s(value_type type);

// NULL terminated array
char **tokenize(char *text, size_t len);
