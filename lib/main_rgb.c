#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rgb.h"

void rgb_alim_1(rgb *pixels, int h, int w) {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++)	{
			(pixels + w * j + i)->r = i % 256;
			(pixels + w * j + i)->g = j % 256;
			(pixels + w * j + i)->b = (i * j) % 256;
		}
	}
}

void rgb_alim_2(rgb *pixels, int h, int w) {
	for (int j = 0; j < h; j++) {
		for (int i = 0; i < w; i++) {
			(pixels + w * j + i)->r = 255 * ((float)j / h);
			(pixels + w * j + i)->g = 255 * ((float)i / w);
			(pixels + w * j + i)->b = 255 * ((float)j / h);
		}
	}
}


int main(void) {
	int w = 256;
	int h = 256;
	rgb pixels[h][w];

	rgb_alim_1(&pixels[0][0], h, w);
	rgb_write_ppm(&pixels[0][0], h, w, 255, "result.ppm");

	rgb_alim_2(&pixels[0][0], h, w);
	rgb_write_tga(&pixels[0][0], h, w, "result.tga");


	file_ppm *ppm = rgb_read_ppm("result.ppm");
	rgb_write_ppm(ppm->pixels, ppm->width, ppm->height, ppm->depth, "after_read.ppm");


	return 0;
}

