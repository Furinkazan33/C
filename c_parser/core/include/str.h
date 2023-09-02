#include <stdlib.h>


/*
 * string representation with it's allocated size.
 * Reallocs are done when needed.
 */

typedef struct str {
	size_t alloc_len; // sizeof(char) * (alloc_len + 1)
	char *data;
} str;

str *str_new(size_t alloc_size);
str *str_realloc(str *str, double coef);

str *str_copy(str *str);
str *str_cat(str *str, char *c, size_t len);
str *str_replace(str *s, char *value); //strcpy value
str *str_set(str *s, char *value, size_t alloc_len); // set by ref

str *str_shift(str *str, size_t idx1, int n); // from idx to idx + n
str *str_remove_char(str *s, size_t idx);
str *str_insert_char(str *s, size_t idx, char c);

str *str_normalize(str *s, char escape_char);
str *str_escape(str *s, const char *list, char escape_char);

void str_write(str *s, FILE *file);

void str_free(str *str);

