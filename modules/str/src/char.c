#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "char.h"


/*
 * Abreviations used :
 *  - bow : begin of word
 *  - eow : end of word
 *  - daw : delete arround word (eword)
 *
 * An expanded word (eword) is a word with ahead or following spaces
 * depending of the context.
 *
 * */


char char_separators[] = {
	'&', '~', '"', '#', '\'', '{', '(', '[',
	'-', '|', '`', '_', '\\', '@', ')', ']',
	'=', '}', '+', '$', '%', '*', ',', '?',
	';', '.', ':', '/', '!', '<', '>'
};

char char_blanks[] = {
	' ', '\t', '\n', '\r', '\b'
};




/*
 * helpers
 * */


char_type char_get_type(char c) {
	size_t i;

	for(i = 0; i < sizeof(char_separators); i++) {
		if(c == char_separators[i]) {
			return SEPARATOR;
		}
	}
	for(i = 0; i < sizeof(char_blanks); i++) {
		if(c == char_blanks[i]) {
			return BLANK;
		}
	}
	return WORD;
}

int char_is_blank(char c) {
	return char_get_type(c) == BLANK;
}

int char_is_word(char c) {
	return char_get_type(c) == WORD;
}

int char_is_separator(char c) {
	return char_get_type(c) == SEPARATOR;
}

/* on the last char of the word */
int char_is_eow(char *start, char *end, char *c) {
	if(c == start || char_is_blank(*c)) {
		return 0;
	}
	if(c+1 == end || char_is_blank(*(c+1)) || char_is_separator(*(c+1))) {
		return 1;
	}
	return 0;
}

/* on the first char of the word */
int char_is_bow(char *start, char *end, char *c) {
	if(c == end || char_is_blank(*c)) {
		return 0;
	}
	if(c == start || char_is_blank(*(c-1)) || char_is_separator(*(c-1))) {
		return 1;
	}
	return 0;
}

void _set_boundaries(char *start, char *end, char *from, char *to) {
	if(from > to) {
		char *tmp = to;
		to = from;
		from = tmp;
	}
	if(from < start) {
		from = start;
	}
	if(from > end) {
		from = end;
	}
	if(to < start) {
		to = start;
	}
	if(to > end) {
		to = end;
	}
}



/*
 * MOVING functions
 * Functions with no modification on the parameters
 *
 * */

char *char_ignore_blanks(char *start, char *end, char *c, int step) {
	char *stop;
	char *res = c;

	if(step > 0) {
		stop = end;
	}
	else {
		stop = start;
	}

	while(res != stop && char_is_blank(*res)) {
		res+=step;
	}
	return res;
}

char *char_ignore_word(char *start, char *end, char *c, int step) {
	char *stop;
	char *res = c;

	if(step > 0) {
		stop = end;
	}
	else {
		stop = start;
	}
	while(res != stop && char_is_word(*res)) {
		res+=step;
	}
	return res;
}

/* first char of current eword */
char *char_bow(char *start, char *end, char *c) {
	char *res = c;

	if(res == start || char_is_separator(*res)) {
		return res;
	}
	if(char_is_blank(*res)) {
		res = char_ignore_blanks(start, end, res, -1);
		res = char_ignore_word(start, end, res, -1);
	}
	else if(char_is_word(*res)) {
		res = char_ignore_word(start, end, res, -1);
	}

	if(res != start) {
		res++;
	}

	return res;
}

/* last char of current eword */
char *char_eow(char *start, char *end, char *c) {
	char *res = c;

	if(char_is_blank(*res)) {
		res = char_ignore_blanks(start, end, res, 1);

		if(char_is_word(*res)) {
			res = char_ignore_word(start, end, res, 1);
		}
	}

	else if(char_is_word(*res)) {
		res = char_ignore_word(start, end, res, 1);
	}

	else if(char_is_separator(*res)) {
		res++;
	}

	res--;

	return res;
}

/* like char_bow, moving to the previous word when already at the bow */
char *char_bow_r(char *start, char *end, char *c) {
	char *res = c;

	if(res == start) {
		return res;
	}
	if(char_is_bow(start, end, res)) {
		res--;
		return char_bow_r(start, end, res);
	}
	return char_bow(start, end, res);
}

/* like char_eow, moving to the next word when already at the eow */
char *char_eow_r(char *start, char *end, char *c) {
	char *res = c;

	if(res == end) {
		return res - 1;
	}
	if(char_is_eow(start, end, res)) {
		res++;
		return char_eow_r(start, end, res);
	}
	return char_eow(start, end, res);
}

/* first space char */
char *char_bos(char *start, char *end, char *c) {
	assert(char_is_blank(*c));

	char *res = char_ignore_blanks(start, end, c, -1);

	if(res != start) {
		res++;
	}
	return res;
}

/* last space char */
char *char_eos(char *start, char *end, char *c) {
	assert(char_is_blank(*c));

	char *res = char_ignore_blanks(start, end, c, 1);
	res--;

	return res;
}



/*
 * EDIT functions
 * Functions that modify the pointed parameter string
 *
 * */

/* Delete with boundaries verification */
void char_delete_to(char *start, char *end, char *from, char *to) {
	_set_boundaries(start, end, from, to);

	if(from > to) {
		memcpy(to, from, strlen(from) + 1);
	}
	else {
		memcpy(from, to, strlen(to) + 1);
	}
}

/* Delete just the current word */
int char_dw(char *start, char *end, char *c) {
	char *res = c;

	assert(res != end);

	char *to = char_bow(start, end, c);
	char *from = char_eow(start, end, c);
	from++;

	char_delete_to(start, end, from, to);

	return 1;
}

/* Delete the current eword with :
 * - following spaces when cursor is on the word
 * - ahead spaces when cursor is on them */
int char_daw(char *start, char *end, char *c) {
	char *res = c;
	char *from, *to;

	assert(res != end);

	if(char_is_blank(*res)) {
		from = char_bos(start, end, c);
		to = char_eow(start, end, c);
		to++;
	}
	else {
		from = char_bow(start, end, c);
		to = char_eow(start, end, c);
		to++;
		if(char_is_blank(*to)) {
			to = char_eos(start, end, to);
			to++;
		}
	}

	char_delete_to(start, end, from, to);

	return 1;
}



