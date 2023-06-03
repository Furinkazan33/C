#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define MAX_TEXT_SIZE 10000

void text_print(char *text, int len);
void words_print(char **words);
void text_split(char *text, char token, char **words);
