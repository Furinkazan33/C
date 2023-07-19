#include <stdio.h>
#include <stdlib.h>
#include "cursor.h"

void cursor_init(cursor *c, int id, char *name, int l, char *bol, char *p, char *eol) {
	c->id = id;
	strncpy(c->name, name, CUR_NAME_LEN);
	c->bol = bol;
	c->p = p;
	c->eol = eol;
	c->l = l;
}

int str_is_blank(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\b';
}

void str_ignore_blanks(char *start, char **t, int step, char *end) {
	char *stop;

	if(step > 0) {
		stop = end;
	}
	else {
		stop = start;
	}
	while(*t != stop && str_is_blank(**t)) {
		(*t)+=step;
	}
}

void str_ignore_non_blanks(char *start, char **t, int step, char *end) {
	char *stop;

	if(step > 0) {
		stop = end;
	}
	else {
		stop = start;
	}
	while(*t != stop && !str_is_blank(**t)) {
		(*t)+=step;
	}
}

/* next word or false */
int str_next_word(char *start, char **t, char *end) {
	char *p = *t;

	str_ignore_non_blanks(start, t, 1, end);
	str_ignore_blanks(start, t, 1, end);

	if(*t == end) {
		*t = p;
		return 0;
	}
	return 1;
}

void str_eow(char *start, char **t, char *end) {
	str_ignore_non_blanks(start, t, 1, end);

	if(*t != end) {
		(*t)--;
	}
}

void str_bow(char *start, char **t, char *end) {
	str_ignore_blanks(start, t, -1, end);
	str_ignore_non_blanks(start, t, -1, end);

	if(*t != start) {
		(*t)++;
	}
}

/* prev word or false */
int str_prev_word(char *start, char **t, char *end) {
	char *p = *t;

	str_ignore_non_blanks(start, t, -1, end);
	str_ignore_blanks(start, t, -1, end);

	if(*t == start) {
		*t = p;
		return 1;
	}
	str_bow(start, t, end);
	return 0;
}

void str_eol(char **t, char *end) {
	while(*t != end && **t != '\n') {
		(*t)++;
	}
	if(*t != end) {
		(*t)--;
	}
}

void str_bol(char *start, char **t) {
	while(*t != start && **t != '\n') {
		(*t)--;
	}
	if(*t != start) {
		(*t)++;
	}
}

