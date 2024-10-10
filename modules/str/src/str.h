#include <stdlib.h>


/*
 * string representation with it's allocated size.
 * Reallocs are done when needed.
 */

#define STR_INIT_ALLOC_LEN 16

typedef struct str {
	size_t alloc_len; // sizeof(char) * (alloc_len + 1)
	char *data;
} str;

int str_cmp(void *s1, void *s2);
void str_write(void *s, FILE *file);
void str_free(void *s);

str *str_new(char *s);
str *str_realloc(str *str, double coef);
str *str_set(str *s, char *value, size_t alloc_len); // set by ref
str *str_replace(str *s, char *value); //strcpy value

str *str_cat(str *str, char *c, size_t len);
str *str_cats(str *dest, str *src);

str *str_shift(str *str, size_t idx1, int n); // from idx to idx + n
str *str_remove_char(str *s, size_t idx);
str *str_insert_char(str *s, size_t idx, char c);

str *str_insert(str *dest, size_t position, char *src);
str *str_inserts(str *dest, size_t position, str *src);

str *str_normalize(str *s, char escape_char);
str *str_escape(str *s, const char *list, char escape_char);

char **str_tokenize(str *s); // with spaces

