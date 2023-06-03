#include <stdlib.h>
#include <string.h>
#include <text.h>

#define _POSIX_C_SOURCE 200809L

#define MAX_WORDS_PER_LINE 100


int main(void) {
	char *text = "un;petit;cheval;gris;en;fonte";
	char *words[MAX_WORDS_PER_LINE];
	text_split(text, ';', words);
	words_print(words);
	
	return 0;
}