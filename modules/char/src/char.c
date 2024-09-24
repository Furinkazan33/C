#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "char.h"


/*
 * Abreviations used :
 *  - bow : begin of word
 *  - eow : end of word
 *  - dw : delete word
 *  - daw : delete arround word (eword)
 *
 * An expanded word (eword) is a word with ahead or following spaces
 * depending of the context.
 *
 * */

int char_is_word(int c) {
	return c == '_' || islower(c) || isupper(c);
}

int char_is_separator(int c) {
	return !char_is_word(c) && !isspace(c);
}

/* on the last char of a word */
int char_is_eow(char *p) {
	assert(p);

	if(!char_is_word(*p)) { return 0; }
	if(char_is_word(*(p+1))) { return 0; }
	return 1;
}

/* on the first char of the word */
int char_is_bow(char *s, char *c) {
	assert(s);
	assert(c);

	if(!char_is_word(*c)) { return 0; }
	if(c-1 >= s && char_is_word(*(c-1))) { return 0; }
	return 1;
}


/*
 * MOVING functions
 * Functions with no modification on the parameters
 *
 * */

char *char_ignore_left(char *s, char *p) {
	assert(s);
	assert(p);
	int (*_is_)(int) = NULL;
	if(isspace(*p)) { _is_ = isspace; }
	else if(char_is_word(*p)) { _is_ = char_is_word; }
	else if(char_is_separator(*p)) { _is_ = char_is_separator; }
	while(*p && p > s && _is_(*p)) { p--; }
	return p;
}

char *char_ignore_right(char *p) {
	assert(p);
	int (*_is_)(int) = NULL;
	if(isspace(*p)) { _is_ = isspace; }
	else if(char_is_word(*p)) { _is_ = char_is_word; }
	else if(char_is_separator(*p)) { _is_ = char_is_separator; }
	while(*p && _is_(*p)) { p++; }
	return p;
}



/* first char of current eword */
char *char_bow(char *s, char *p) {
	assert(p);

	if(char_is_separator(*p)) {
		while(p != s && char_is_separator(*p)) p--;
		if(p != s) p++;
	}
	else if(isspace(*p)) {
		while(p != s && isspace(*p)) p--;
		if(p != s) return char_bow(s, p);
	}
	else if(char_is_word(*p)) {
		while(p != s && char_is_word(*p)) p--;
		if(p != s) p++;
	}
	return p;
}

/* last char of current eword */
char *char_eow(char *c) {
	if(isspace(*c)) {
		c = char_ignore_right(c);
		if(char_is_word(*c)) { c = char_ignore_right(c); }
	}
	else if(char_is_word(*c)) {  c = char_ignore_right(c); }
	else if(char_is_separator(*c)) { c++; }

	return --c;
}

/* like char_bow, moving to the previous word when already at the bow */
char *char_bow_r(char *start, char *c) {
	char *res = c;

	if(res == start) {
		return res;
	}
	if(char_is_bow(start, res)) {
		res--;
		return char_bow_r(start, res);
	}
	return char_bow(start, res);
}

/* like char_eow, moving to the next word when already at the eow */
char *char_eow_r(char *c) {
	char *res = c;

	if(char_is_eow(res)) {
		res++;
		return char_eow_r(res);
	}
	return char_eow(res);
}

/* first space char */
char *char_bos(char *start, char *c) {
	assert(isspace(*c));

	char *res = char_ignore_left(start, c);

	if(res != start) {
		res++;
	}
	return res;
}

/* last space char */
char *char_eos(char *c) {
	assert(isspace(*c));

	char *res = char_ignore_right(c);
	res--;

	return res;
}



/*
 * EDIT functions
 * Functions that modify the pointed parameter string
 *
 * */

void char_delete_range(char *from, char *to) {
	if(from > to) {
		char *tmp = from;
		from = to;
		to = tmp;
	}
	memcpy(from, to, strlen(to) + 1);
}

/* Delete just the current word */
void char_dw(char *s, char *c) {
	assert(s);
	assert(char_is_word(*c));

	char *from = char_bow(s, c);
	char *to = char_eow(c);

	char_delete_range(from, to);
}

/* Delete the current eword with :
 * - following spaces when cursor is on the word
 * - ahead spaces when cursor is on them */
int char_daw(char *start, char *c) {
	char *res = c;
	char *from, *to;

	if(isspace(*res)) {
		from = char_bos(start, c);
		to = char_eow(c);
		to++;
	}
	else {
		from = char_bow(start, c);
		to = char_eow(c);
		to++;
		if(isspace(*to)) {
			to = char_eos(to);
			to++;
		}
	}

	char_delete_range(from, to);

	return 1;
}



