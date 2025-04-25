#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

void str_shift_left(char *s, size_t n) {
	size_t len = strlen(s);
	for(size_t i = 0; s[i] && i + n <= len; i++) {
		s[i] = s[i + n];
	}
}

/*
 * warning : if start + amount > strlen(s), resulting string will contain '\0'
 */
char *str_shift_right(char *s, size_t start, size_t amount) {
	char *tmp = realloc(s, sizeof(char) * (strlen(s) + amount + 1));
	if(!tmp) {
		perror("realloc failed");
		fprintf(stderr, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, "failed to realloc string");
		return NULL;
	}
	s = tmp;

	{
		/* tmp string to avoid copying corrupted values */
		tmp = malloc(sizeof(char) * (strlen(s) + 1));
		if(!tmp) {
			perror("malloc failed");
			fprintf(stderr, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, "failed to alloc string");
			return NULL;
		}
		strcpy(tmp, s + start);
	}

	strcpy(s + start + amount, tmp);

	free(tmp);

	return s;
}

char *str_shift(char *s, size_t start, int amount) {
	if(amount < 0) {
		str_shift_left(s + start, -amount);
		return s;
	}
	else {
		return str_shift_right(s, start, amount);
	}
}

char *str_insert(char *dest, size_t position, char *src) {
	if(!str_shift(dest, position, strlen(src))) {
		fprintf(stderr, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, "failed to shift string");
		return NULL;
	}

	strncpy(dest + position, src, strlen(src));

	return dest;
}

char *str_next_blank(char *start) {
	while(start && *start && !isblank(*start)) { start++; }
	return start;
}

char *str_next_non_blank(char *start) {
	while(start && *start && isblank(*start)) { start++; }
	return start;
}

char *str_next_word(char *start) {
	if(!isblank(*start)) {
		start = str_next_blank(start);
	}
	return str_next_non_blank(start);
}

// end of current word
char *str_end(char *start) {
	if(!isblank(*start)) {
		while(!isblank(*start) && start && *start) {
			start++;
		}
	}
	else {
		while(isblank(*start) && start && *start) {
			start++;
		}
	}
	start--;

	return start;
}

// begin of current word or blanks
char *str_begin(char *begin, char *start) {
	if(!isblank(*start)) {
		while(start && *start && start > begin && !isblank(*start)) { start--; }
		if(isblank(*start)) { 
			start++;
		}
	}
	else {
		while(start && *start && start > begin && isblank(*start)) { start--; }
		if(!isblank(*start)) { 
			start++;
		}
	}
	return start;
}

