
#define N_MAX_TOK 25
#define L_MAX_LINE 100
#define EXIT_NOT_FOUND -1


str_len(char *src);

char *str_copy(char *src);

void str_print(char *src);

// Returns the index of the first occurence of the given pattern or -1
int str_idx(char *source, char *pattern);

// Shift string left or right from idx
void str_shift(char *src, int idx, int len);

// Replace the first iteration of the given pattern
// end of src string is lost if rpl length gt pattern length
int str_rpl(char *src, char *pattern, char *rpl);

int str_split(char *struct_def, char delim[], char **result);

char *str_join(char *defs[], char delim, int end);


