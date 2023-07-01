#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct rgb {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} rgb;

const rgb rgb_white = {255, 255, 255};
const rgb rgb_blue = {0, 0, 255};

void rgb_print(char *string, rgb *c) {
	printf("\x1b[38;2;%d;%d;%dm%s", c->r, c->g, c->b, string);
	printf("\x1b[38;2;%d;%d;%dm", (&rgb_white)->r, (&rgb_white)->g, (&rgb_white)->b);
}

void rgb_print_rgb(char *string, uint8_t r, uint8_t g, uint8_t b) {
	rgb c = { r, g, b };
	rgb_print(string, &c);
}

int rgb_write_ppm(rgb *pixels, int h, int w, int depth, char *filepath) {
	FILE *fp = fopen(filepath, "wb"); /* b - binary mode */
	int n;

	if(!fp) {
		perror("Error open file in binary write mode.");
		return -1;
	}

	fprintf(fp, "P6\n%d %d\n%d\n", w, h, depth);
	n = fwrite(pixels, sizeof(rgb) * w * h, 1, fp);

	fclose(fp);

	if(!n) {
		perror("Error writing ppm file content");
		return n;
	}

	return n;
}

typedef struct file_ppm {
	char magic[2];
	int width;
	int height;
	int depth;
	rgb *pixels;
} file_ppm;

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
	int n;

	if(!fp) {
		perror("Error reading ppm file");
		return NULL;
	}

	char r[128];
	n = read_until_space(&r[0], fp); //magic (P6)
	if(n != 2) {
		perror("Error reading magic");
		return NULL;
	}

	n = read_until_space(&r[0], fp); //width
	if(n <= 0) {
		perror("Error reading width");
		return NULL;
	}
	res->width = atoi(r);

	n = read_until_space(&r[0], fp); //height
	if(n <= 0) {
		perror("Error reading height");
		return NULL;
	}
	res->height = atoi(r);

	n = read_until_space(&r[0], fp); //Max color value
	if(n <= 0) {
		perror("Error reading colors depth");
		return NULL;
	}
	res->depth = atoi(r);

	res->pixels = (rgb *)malloc(sizeof(rgb) * res->height * res->width);
	n = fread(res->pixels, sizeof(rgb) * res->height * res->width, 1, fp);

	if(n <= 0) {
		perror("Error reading ppm pixels");
		return NULL;
	}

	return res;
}

int rgb_write_tga(rgb *pixels, int h, int w, char *filepath) {
	FILE *fp = fopen(filepath, "wb"); /* b - binary mode */
	static unsigned char tga[18];
	int n;

	if(!fp) {
		perror("Error open file in binary write mode.");
		return -1;
	}

	tga[2] = 2;
	tga[12] = 255 & w;
	tga[13] = 255 & (w >> 8);
	tga[14] = 255 & h;
	tga[15] = 255 & (h >> 8);
	tga[16] = 24;
	tga[17] = 32;

	if(!fwrite(&tga, sizeof(tga), 1, fp)) {
		perror("Error writing tga file specification");
		return -1;
	}

	n = fwrite(pixels, sizeof(rgb) * w * h, 1, fp);

	fclose(fp);

	if(n < 0) {
		perror("Error writing tga file content");
		return n;
	}

	return n;
}

