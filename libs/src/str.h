#include <stdlib.h>


void str_escape(char *dest, char *src, char c);
void str_unescape(char *str);

char **str_split(char *str, char delim, size_t *len);

void str_cats(char *dest, size_t *alloc, char *strings[]);

void str_shift_left(char *s, size_t amount);
char *str_shift_right(char *s, size_t start, size_t amount);
char *str_shift(char *s, size_t start, int amount);
char *str_insert(char *dest, size_t position, char *src);

char *str_next_blank(char *start);
char *str_next_non_blank(char *start);
char *str_end(char *start);
char *str_begin(char *begin, char *start);
char *str_next_word(char *start);


