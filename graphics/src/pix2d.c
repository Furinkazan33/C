#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <helpers.h>
#include "pix2d.h"

rgba rgba_black = { 0, 0, 0, 255 };
rgba rgba_white = { 255, 255, 255, 255 };
rgba rgba_red = { 255, 0, 0, 255 };
rgba rgba_blue = { 0, 0, 255, 255 };

void ppm_free(ppm *file) {
	if(file->pixels) {
		free(file->pixels);
	}
	free(file);
}

uint8_t *ppm_write(char *filepath, size_t depth, size_t w, size_t h, uint8_t *pixels) {
	FILE *fd;

	fd = fopen(filepath, "wb");
	IF_PERROR(!fd, "fopen failed", );

	fprintf(fd, "P6\n%ld %ld\n%ld\n", w, h, depth);

	// rgba to rgb
	int n;
	FOR(i, 0, h * w, { 
			n = fwrite(pixels + i * sizeof(rgba), 3 * sizeof(uint8_t), 1, fd);
			if(n != 1) { perror("ppm_write : fwrite error"); fclose(fd); return NULL; }
			})

	fclose(fd);

	return pixels;
}

int read_until_space(char *result, FILE *fd) {
	int i = 0;

	for(char c = fgetc(fd); c != ' ' && c != '\0' && c != '\n'; i++) {
		result[i] = c;
		c = fgetc(fd);
	}

	return i;
}

ppm *ppm_read(ppm *dest) {
	FILE *fd;
	size_t n;
	char width[16];
	char height[16];
	char depth[16];

	fd = fopen(dest->path, "rb");
	IF_PERROR(!fd, "fopen failed", );

	n = read_until_space(dest->magic, fd); //magic (P6)
	IF_RNULL(n != 2, "error reading magic", fclose(fd));

	n = read_until_space(&width[0], fd); //width
	IF_RNULL(n <= 0, "error reading width", fclose(fd));

	n = read_until_space(&height[0], fd); //height
	IF_RNULL(n <= 0, "error reading height", fclose(fd));

	n = read_until_space(&depth[0], fd); //Max color value
	IF_RNULL(n <= 0, "error reading colors depth", fclose(fd));

	dest->depth = atoi(depth);

	// ppm file pixels are rgb
	dest->pixels = malloc(3 * sizeof(uint8_t) * atoi(width) * atoi(height));
	IF_PERROR(!dest->pixels, "malloc failed", fclose(fd));

	fclose(fd);

	return dest;
}

void rgba_draw_rect(uint8_t *pixels, size_t w, rectangle *r, rgba *color) {
	FFOR(l, 0, r->h, c, 0, r->w, memcpy(pixels + sizeof(rgba) * (l * w + c), color, sizeof(rgba));)
}

size_t d2(size_t x0, size_t y0, size_t x1, size_t y1) {
	return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

void rgba_draw_circle(uint8_t *pixels, size_t w, circle *ci, rgba *color) {
	FFOR(l, 0, 2 * ci->r, c, 0, 2 * ci->r, if(d2(ci->x, ci->y, c + ci->x - ci->r, l + ci->y - ci->r) <= ci->r * ci->r) memcpy(pixels + sizeof(rgba) * (l * w + c), color, sizeof(rgba));)
}

