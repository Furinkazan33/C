#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
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

char *char_separators = " &~#{([-|`@)]=}+$%*?;.:/!<>'\\\"\t\n\r\b";
char *char_blanks = " \t\n\r\b";



/*
 * helpers
 * */


char_type char_get_type(char c) {
	if(strchr(char_separators, c)) {
		return CT_SEPARATOR;
	}
	if(strchr(char_blanks, c)) {
		return CT_BLANK;
	}
	return CT_WORD;
}

bool char_is_blank(char c) {
	return char_get_type(c) == CT_BLANK;
}

bool char_is_word(char c) {
	return char_get_type(c) == CT_WORD;
}

bool char_is_separator(char c) {
	return char_get_type(c) == CT_SEPARATOR;
}

/* on the last char of a word */
bool char_is_eow(char *p) {
	assert(p);

	if(!char_is_word(*p)) {
		return false;
	}
	if(char_is_word(*(p+1))) {
		return false;
	}
	return true;
}

/* on the first char of the word */
bool char_is_bow(char *s, char *c) {
	assert(s);
	assert(c);

	if(!char_is_word(*c)) {
		return false;
	}
	if(c-1 >= s && char_is_word(*(c-1))) {
		return false;
	}
	return true;
}


/*
 * MOVING functions
 * Functions with no modification on the parameters
 *
 * */

char *char_ignore(char *s, char *p, int step) {
	assert(p);

	char *res = p;
	bool (*_is_)(char) = NULL;

	if(char_is_blank(*p)) {
		_is_ = char_is_blank;
	}
	else if(char_is_word(*p)) {
		_is_ = char_is_word;
	}

	while(*res && res >= s && _is_(*res)) {
		res+=step;
	}
	return res;
}


/* first char of current eword */
char *char_bow(char *s, char *p) {
	assert(s);
	assert(p);

	char *res = NULL;

	if(p == s || char_is_separator(*p)) {
		return p;
	}
	if(char_is_blank(*p)) {
		res = char_ignore(s, res, -1);
		res = char_ignore(s, res, -1);
	}
	else if(char_is_word(*res)) {
		res = char_ignore(s, res, -1);
	}

	if(res != s) {
		res++;
	}

	return res;
}

/* last char of current eword */
char *char_eow(char *c) {
	char *res = c;

	if(char_is_blank(*res)) {
		res = char_ignore(NULL, res, 1);

		if(char_is_word(*res)) {
			res = char_ignore(NULL, res, 1);
		}
	}

	else if(char_is_word(*res)) {
		res = char_ignore(NULL, res, 1);
	}

	else if(char_is_separator(*res)) {
		res++;
	}

	res--;

	return res;
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
	assert(char_is_blank(*c));

	char *res = char_ignore(start, c, -1);

	if(res != start) {
		res++;
	}
	return res;
}

/* last space char */
char *char_eos(char *c) {
	assert(char_is_blank(*c));

	char *res = char_ignore(NULL, c, 1);
	res--;

	return res;
}



/*
 * EDIT functions
 * Functions that modify the pointed parameter string
 *
 * */

void char_delete_range(char *from, char *to) {
	char *tmp = NULL;
	if(from < to) {
		tmp = from;
		from = to;
		to = tmp;
	}

	memcpy(from, to + 1, strlen(to));
}

/* Delete just the current word */
void char_dw(char *s, char *c) {
	assert(s);
	assert(c);

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

	if(char_is_blank(*res)) {
		from = char_bos(start, c);
		to = char_eow(c);
		to++;
	}
	else {
		from = char_bow(start, c);
		to = char_eow(c);
		to++;
		if(char_is_blank(*to)) {
			to = char_eos(to);
			to++;
		}
	}

	char_delete_range(from, to);

	return 1;
}



