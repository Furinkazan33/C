#include <linux/input.h> //for keys definitions

#define KB_MAX_KEYS 128

typedef struct keyboard {
	char *path;
	int fd;
	int count;
	int pressed[KB_MAX_KEYS];
} keyboard;


/* try opening first keyboard found */
keyboard *kbd_open(char *folder);

/* read keyboard events and set pressed keys */
keyboard *kbd_read(keyboard *k);

/* is any key pressed ? */
int kbd_any(keyboard *k);

/* set all keys stored in pressed to false */
void kbd_reinit(keyboard *k);

/* write keyboard values to file */
void kbd_printf(keyboard *k);
void kbd_fprintf(FILE *f, keyboard *k);

/* free allocated memory and close fd */
void kbd_free(keyboard *k);


