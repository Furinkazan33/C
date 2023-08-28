#include <stdlib.h>


/*
 * string representation with it's allocated size and length.
 *
 */

typedef struct str {
	size_t alloc_len; // alloc size is sizeof(char) * (alloc_len + 1)
	size_t n;
	char *data;
} str;



str *str_new(size_t alloc_size);
void str_free(str *str);

/* If realloc failed, str is unchanged. */
str *str_realloc(str *str, double coef);

str *str_copy(str *str);

/* realloc if needed */
str *str_cat(str *str, char *c, size_t len);

/* shift from idx to idx + n. Realloc if needed. */
str *str_shift(str *str, size_t idx1, int n);

/* set value, alloc_len and n */
str *str_set(str *s, char *value);
str *str_set_byref(str *s, char *value, size_t alloc_len);

