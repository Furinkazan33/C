#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "kbd.h"


int main(int argc, char **argv)
{
	if(argc != 1) {
		printf("usage : %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	keyboard *k = kbd_open("/dev/input/by-path/");

	while(1) {
		kbd_read(k);
#ifdef DEBUG
	kbd_write(k, stdout);
#endif
		if(k->pressed[KEY_LEFTCTRL] && k->pressed[KEY_Q]) {
			puts("CTRL A");
		}
	}

	kbd_free(k);


	return EXIT_SUCCESS;
}

