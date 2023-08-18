#include <linux/input.h>
#include <stdbool.h>

/* Size of the pressed bool array */
#define KB_MAX_KEYS 128


/* Toggle debug */
void hwi_debug();

/* Find keyboard file in given folder, open it and return fd or -1 */
int hwi_open(char *folder);

/* Read keyboard inputs into pressed bool array.
 * pressed must be initialize to false before the first call to this function. 
 * Return 0 if error, errno is set */
int hwi_read(int fd, struct input_event ev, bool *pressed);

void hwi_close(int fd);

