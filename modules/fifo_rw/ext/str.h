#include <stdlib.h>

void str_shift_left(char *s, size_t amount);
char *str_shift_right(char *s, size_t start, size_t amount);
char *str_shift(char *s, size_t start, int amount);
char *str_insert(char *dest, size_t position, char *src);

char *str_next_blank(char *start);
char *str_next_non_blank(char *start);
char *str_end(char *start);
char *str_begin(char *begin, char *start);
char *str_next_word(char *start);


