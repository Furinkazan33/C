#include <string.h>
#include <ctype.h>
#include <assert.h>

char *w_next_blank(char *start) {
	assert(!isblank(*start));
	while(start && *start && !isblank(*start)) { start++; }
	return start;
}

char *w_next_non_blank(char *start) {
	assert(isblank(*start));
	while(start && *start && isblank(*start)) { start++; }
	return start;
}

char *w_end(char *start) {
	assert(!isblank(*start));
	return w_next_blank(start) - 1;
}

char *w_start(char *begin, char *start) {
	assert(!isblank(*start));
	while(start && *start && start > begin && !isblank(*start)) { start--; }

	if(isblank(*start)) { 
		start++;
	}
	return start;
}

char *w_next_word(char *start) {
	if(!isblank(*start)) {
		start = w_next_blank(start);
	}
	return w_next_non_blank(start);
}

