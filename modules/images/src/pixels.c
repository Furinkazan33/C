#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "pixels.h"


canvas *cs_new(size_t width, size_t height, size_t depth) {
	canvas *res = malloc(sizeof(*res));
	if(!res) {
		return NULL;
	}
	res->pixels = malloc(sizeof(*(res->pixels)) * width * height);
	if(!res->pixels) {
		return NULL;
	}
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
void _print_as_rgb(char *string, uint8_t r, uint8_t g, uint8_t b) {
	rgb c = { r, g, b };
	rgb_print(string, &c);
}
ppm *ppm_new(char *path, canvas *cs) {
	ppm *res = malloc(sizeof(*res));
	if(!res) {
		return NULL;
	}
	strcpy(res->path, path);
	res->cs = cs;

	return res;
}

ppm *ppm_write(ppm *file) {
	FILE *fp = fopen(file->path, "wb");
	size_t n;

	if(!fp) {
		perror("ppm_write : fopen failed");
		return NULL;
	}

	fprintf(fp, "P6\n%ld %ld\n%ld\n", file->cs->width, file->cs->height, file->cs->depth);
	n = fwrite(file->cs->pixels, sizeof(rgb), file->cs->width * file->cs->height, fp);
	fclose(fp);

	if(n != file->cs->width * file->cs->height) {
		fprintf(stderr, "ppm_write : error writing pixels : %ld != %ld\n", n, file->cs->width * file->cs->height);
		return NULL;
	}

	return file;
}

int read_until_space(char *result, FILE *fp) {
	int i = 0;

	for(char c = fgetc(fp); c != ' ' && c != '\0' && c != '\n'; i++) {
		result[i] = c;
		c = fgetc(fp);
	}

	return i;
}

ppm *ppm_read(ppm *file) {
	FILE *fp = fopen(file->path, "rb");
	size_t n;
	char width[128];
	char height[128];
	char depth[128];

	if(!fp) {
		perror("ppm_read : fopen failed");
		return NULL;
	}

	n = read_until_space(file->magic, fp); //magic (P6)
	if(n != 2) {
		fprintf(stderr, "ppm_read : error reading magic\n");
		return NULL;
	}

	n = read_until_space(&width[0], fp); //width
	if(n <= 0) {
		fprintf(stderr, "ppm_read : error reading width\n");
		return NULL;
	}

	n = read_until_space(&height[0], fp); //height
	if(n <= 0) {
		fprintf(stderr, "ppm_read : error reading height\n");
		return NULL;
	}

	n = read_until_space(&depth[0], fp); //Max color value
	if(n <= 0) {
		fprintf(stderr, "ppm_read : error reading colors depth\n");
		return NULL;
	}

	file->cs = cs_new(atoi(width), atoi(height), atoi(depth));
	n = fread(file->cs->pixels, sizeof(rgb), file->cs->height * file->cs->width, fp);

	if(n != file->cs->height * file->cs->width) {
		fprintf(stderr, "ppm_read : error reading pixels : %ld != %ld\n",
				n, file->cs->height * file->cs->width);
		return NULL;
	}

	return file;
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


void circle_print(circle *c) {
	printf("circle(x:%ld y:%ld r:%ld)\n", c->x, c->y, c->r);
}

void cs_draw_circle(canvas *cs, circle *ci, rgb *color) {
	printf("cs_draw_circle : ");
	circle_print(ci);

	for(size_t l = ci->y - ci->r; l <= ci->y + ci->r; l++) {
		for(size_t c = ci->x - ci->r; c <= ci->x + ci->r; c++) {
			if(d2(ci->x, ci->y, c, l) <= ci->r * ci->r) {
				cs_set(cs, l, c, color);
			}
		}
	}
}

void cs_draw_line(canvas *cs, line *li, rgb *color) {
	cs_draw_circle(cs, &((circle) { .x=li->x0, .y=li->y0, .r=5 }), color);
	cs_draw_circle(cs, &((circle) { .x=li->x1, .y=li->y1, .r=5 }), color);

	int dx = li->x1 - li->x0;
	int dy = li->y1 - li->y0;
	float cumul = 0;

	int cinc, linc;
	size_t cstart = li->x0; size_t cend = li->x1; 
	size_t lstart = li->y0; size_t lend = li->y1;
	if(dx >= 0) { cinc = 1; } else { cinc = -1; }
	if(dy >= 0) { linc = 1; } else { linc = -1; }

	if(dy == 0) {
		for(size_t c = cstart, l = lstart; c != cend; c+=cinc) {
			printf("%ld %ld %d\n", c, cend, cinc);
			cs_set(cs, l, c, color);
		}
	}
	else if(dx == 0) {
		for(size_t c = cstart, l = lstart; l != lend; l+=linc) {
			cs_set(cs, l, c, color);
		}
	}
	// on avance plus en x qu'en y
	else if(dx > dy) {
		for(size_t l = lstart, c = cstart; l != lend && c != cend; l+=linc) {
			// le surplus
			cumul += dx / dy;
			while(cumul >= 1) {
				cs_set(cs, l, c, color);
				cumul -= 1;
				c++;
			}
		}
	}
	else {
		for(size_t c = cstart, l = lstart; c != cend && l != lend; c+=cinc) {
			cumul += dy / dx;
			while(cumul >= 1) {
				cs_set(cs, l, c, color);
				cumul -= 1;
				l++;
			}
		}
	}
}



