#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "hwi.h"


int main(int argc, char **argv)
{
	if(argc != 1) {
		printf("usage : %s\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	struct input_event ev;
	static bool pressed[KB_MAX_KEYS];

	int fd = hwi_open("/dev/input/by-path/");
	if(fd == -1) {
		return EXIT_FAILURE;
	}

	//hwi_debug();

	while(1) {
		if(!hwi_read(fd, ev, &pressed[0])) {
			return EXIT_FAILURE;
		}

		if(pressed[KEY_LEFTCTRL] && pressed[KEY_Q]) {
			puts("CTRL A");
		}

	}

	hwi_close(fd);

	fflush(stdout);

	return EXIT_SUCCESS;
}

