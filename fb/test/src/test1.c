#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <fb.h>

int main(int argc, char **argv) {

	if(argc != 2) {
		printf("usage : %s filepath\n", argv[0]);
		return -1;
	}
	char sav[64];
	strcpy(sav, argv[1]);
	strcat(sav, ".sav");

	fb *buf = fb_new(argv[1], "", 32);
	fb_load(buf);
	fb_save(buf, sav);
	fb_print(buf);
	fb_free(buf);


	return 0;
}


