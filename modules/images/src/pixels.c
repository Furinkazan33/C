#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pixels.h"


canvas *cs_new(size_t width, size_t height, size_t depth) {
	canvas *res = malloc(sizeof(*res));
	res->pixels = malloc(sizeof(*(res->pixels)) * width * height);
	res->width = width;
	res->height = height;
	res->depth = depth;
	return res;
}

void cs_free(canvas *cs) {
	free(cs->pixels);
	free(cs);
}

rgb rgb_new(uint8_t r, uint8_t g, uint8_t b) {
	return (rgb) { .r=r, .g=g, .b=b };
}

rgb _rgb_white = {255, 255, 255};
rgb _rgb_blue = {0, 0, 255};
rgb _rgb_red = {255, 0, 0};

rgb *rgb_white() { return &_rgb_white; }
rgb *rgb_blue() { return &_rgb_blue; }
rgb *rgb_red() { return &_rgb_red; }

void rgb_print(char *string, rgb *c) {
	printf("\x1b[38;2;%d;%d;%dm%s", c->r, c->g, c->b, string);
}

void rgb_print_rgb(char *string, uint8_t r, uint8_t g, uint8_t b) {
	rgb c = { r, g, b };
	rgb_print(string, &c);
}

int cs_write_ppm(canvas *cs, char *filepath) {
	FILE *fp = fopen(filepath, "wb"); /* b - binary mode */
	size_t n;

	if(!fp) {
		perror("Error open file in binary write mode.");
		return -1;
	}

	fprintf(fp, "P6\n%ld %ld\n%ld\n", cs->width, cs->height, cs->depth);
	n = fwrite(cs->pixels, sizeof(rgb), cs->width * cs->height, fp);

	fclose(fp);

	if(n != cs->width * cs->height) {
		perror("Error writing ppm file content");
		return n;
	}

	return n;
}

int read_until_space(char *result, FILE *fp) {
	int i = 0;

	for(char c = fgetc(fp); c != ' ' && c != '\0' && c != '\n'; i++) {
		result[i] = c;
		c = fgetc(fp);
	}

	return i;
}

file_ppm *rgb_read_ppm(char *filepath) {
	FILE *fp = fopen(filepath, "rb"); /* b - binary mode */
	file_ppm *res = (file_ppm *) malloc(sizeof(file_ppm));
	size_t n;
	char width[128];
	char height[128];
	char depth[128];

	if(!fp) {
		perror("Error reading ppm file");
		return NULL;
	}

	n = read_until_space(res->magic, fp); //magic (P6)
	if(n != 2) {
		perror("Error reading magic");
		return NULL;
	}

	n = read_until_space(&width[0], fp); //width
	if(n <= 0) {
		perror("Error reading width");
		return NULL;
	}

	n = read_until_space(&height[0], fp); //height
	if(n <= 0) {
		perror("Error reading height");
		return NULL;
	}

	n = read_until_space(&depth[0], fp); //Max color value
	if(n <= 0) {
		perror("Error reading colors depth");
		return NULL;
	}

	res->cs = cs_new(atoi(width), atoi(height), atoi(depth));
	n = fread(res->cs->pixels, sizeof(rgb), res->cs->height * res->cs->width, fp);

	if(n != res->cs->height * res->cs->width) {
		perror("Error reading ppm pixels");
		return NULL;
	}

	return res;
}

int cs_write_tga(canvas *cs, char *filepath) {
	FILE *fp = fopen(filepath, "wb"); /* b - binary mode */
	static unsigned char tga[18];
	size_t n;

	if(!fp) {
		perror("Error open file in binary write mode.");
		return -1;
	}

	tga[2] = 2;
	tga[12] = 255 & cs->width;
	tga[13] = 255 & (cs->width >> 8);
	tga[14] = 255 & cs->height;
	tga[15] = 255 & (cs->height >> 8);
	tga[16] = 24;
	tga[17] = 32;

	if(!fwrite(&tga, sizeof(tga), 1, fp)) {
		perror("Error writing tga file specification");
		return -1;
	}

	n = fwrite(cs->pixels, sizeof(rgb), cs->width * cs->height, fp);

	fclose(fp);

	if(n != cs->width * cs->height) {
		perror("Error writing tga file content");
		return n;
	}

	return n;
}

void cs_set(canvas *cs, size_t line, size_t col, rgb *color) {
	(cs->pixels + line * cs->width + col)->r = color->r;
	(cs->pixels + line * cs->width + col)->g = color->g;
	(cs->pixels + line * cs->width + col)->b = color->b;
}

void cs_draw_rect(canvas *cs, rectangle *r, rgb *color) {
	for(size_t l = r->y; l < r->y + r->h; l++) {
		for(size_t c = r->x; c < r->x + r->w; c++) {
			cs_set(cs, l, c, color);
		}
	}
}

size_t d2(size_t x0, size_t y0, size_t x1, size_t y1) {
	return (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
}

void cs_draw_circle(canvas *cs, circle *ci, rgb *color) {
	for(size_t l = ci->y - ci->r; l < ci->y + ci->r; l++) {
		for(size_t c = ci->x - ci->r; c < ci->x + ci->r; c++) {
			if(d2(ci->x, ci->y, l, c) < ci->r * ci->r) {
				cs_set(cs, l, c, color);
			}
		}
	}
}

void cs_draw_line(canvas *cs, line *li, rgb *color) {
	int dx = li->x1 - li->x0;
	int dy = li->y1 - li->y0;
	float p = dx / dy;

	for(float l = li->y0, c = li->x0; ; ) {
		if(p >= 1) {
			for(int i = 0; i <= p; i++) {
				cs_set(cs, l, c, color);
				if(dx > 0) { 
					if(c >= li->x1) { break; }
					c++; 
				} else if(dx < 0) { 
					if(c <= li->x1) { break; }
					c--; 
				}
			}
		}
		else {
			for(int i = 0; i <= p; i++) {
				cs_set(cs, l, c, color);
				if(dy > 0) { 
					if(l >= li->y1) { break; }
					l++; 
				} else if(dy < 0) { 
					if(l <= li->y1) { break; }
					l--; 
				}
			}
		}
	}
}



