#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CUR_NAME_LEN 16

typedef struct cursor {
	int id;
	char name[CUR_NAME_LEN];
	int l;					/* line number */
	char *bol;				/* begin of line */
	char *eol;				/* end of line */
	char *p;				/* pointer position */
} cursor;

void cursor_init(cursor *c, int id, char *name, int l, char *bol, char *p, char *eol);

int str_is_blank(char c);
void str_ignore_blanks(char *start, char **t, int step, char *end);
void str_ignore_non_blanks(char *start, char **t, int step, char *end);

void str_eow(char *start, char **t, char *end);
void str_bow(char *start, char **t, char *end);

int str_next_word(char *start, char **t, char *end);
int str_prev_word(char *start, char **t, char *end);

void str_eol(char **t, char *end);
void str_bol(char *start, char **t);

