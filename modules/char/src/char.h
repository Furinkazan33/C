

/*
 * Functions with no mopdification of curent cursor position
 * */

/* next word or NULL */
char *char_next_word(char *c);

/* prev word or NULL */
char *char_prev_word(char *start, char *c);

/* begin of current word or previous word if on blank */
char *char_bow_r(char *start, char *c);

/* end of current word or next word if on blank */
char *char_eow_r(char *c);

/* delete current word */
int char_daw(char *start, char *c);

