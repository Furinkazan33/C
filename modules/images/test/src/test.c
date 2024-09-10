#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pixels.h"

void str_make(char *res, char *strings[]) {
	res[0] = '\0';
	for(size_t i = 0; strings[i]; i++) {
		strcat(res, strings[i]);
	}
}

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stderr, "main : missing working directory\n");
		return 1;
	}
	char file[128] = {0};
	str_make(file, (char *[]){ argv[1], "/", "result_rect.ppm", 0 });
	canvas *cs1 = cs_new(400, 300, 255);

	rectangle r = { .x=50, .y=50, .w=100, .h=25 }; 
	circle c = { .x=150, .y=150, .r=10 };
	line l = { .x0=50, .y0=50, .x1=200, .y1=120 };

	//cs_draw_rect(cs1, &r, rgb_blue());
	cs_draw_circle(cs1, &c, rgb_white());
	cs_draw_line(cs1, &l, rgb_red());

	ppm *f = ppm_new(file, cs1);
	ppm_write(f);

	cs_free(cs1);

	return 0;
}

