#include <stdlib.h>

typedef struct str {
	size_t size;
	char *start;
	char *end;
} str;

void str_free(str *s);
void str_print(str *s);
int str_cmp(str *s1, str *s2);
void str_debug(str *s);

str *str_alloc(size_t init_size);
str *str_new(char *s, size_t len, size_t init_size);

/* realloc if current size if smaller than required min_size.
 * if realloc failed, s is unchanged.
 * */
str *str_realloc(str *s, size_t min_size);

void str_set_end(str *s, int diff);

str *str_copy(str *s);


