#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fb.h>

int main(int argc, char **argv) {

	if(argc != 3) {
		printf("usage : %s loadfilepath savefilepath\n", argv[0]);
		return -1;
	}

	fb *buf = fb_new(argv[1], "Toto", 32);
	if(!buf) { return 1; }

	if(!fb_load(buf)) { return 1; }

	if(!fb_save(buf, argv[2])) { return 1; }

	fb_print(buf);

	fb_free(buf);


	return 0;
}


