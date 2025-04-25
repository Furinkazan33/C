#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "macros.h"
#include "pix2d.h"



int main() {
	size_t w = 400;
	size_t h = 300;
	uint8_t pixels[w * h][4];
	FOR(i, 0, w * h, memcpy(pixels[i], &rgba_white, sizeof(rgba));)

	rectangle r = { 50, 50, 100, 25 }; 
	circle c = { 150, 150, 10 };

	rgba_draw_rect(&pixels[150][0], w, &r, &rgba_red);
	rgba_draw_circle(&pixels[400 * 100 + 150][0], w, &c, &rgba_blue);

	
	ppm_write("build/result.ppm", 255, w, h, &pixels[0][0]);

	return 0;
}

