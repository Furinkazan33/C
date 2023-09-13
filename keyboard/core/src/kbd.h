#include <stdbool.h>
#include <linux/input.h> //for keys definitions

#define KB_MAX_KEYS 128

typedef struct keyboard {
	char *path;
	int fd;
	bool pressed[KB_MAX_KEYS];
	bool repeated[KB_MAX_KEYS];
} keyboard;


/* try opening first keyboard found */
keyboard *kbd_open(char *folder);

/* read keyboard events and set pressed and repeated keys */
keyboard *kbd_read(keyboard *k);

/* set all keys stored in pressed and repeated to false */
void kbd_reinit(keyboard *k);

/* write keyboard values to file */
void kbd_write(keyboard *k, FILE *file);

/* free allocated memory and close fd */
void kbd_free(keyboard *k);


