#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef struct rgb { uint8_t r; uint8_t g; uint8_t b; } rgb;
typedef struct { rgb *pixels; size_t width; size_t height; size_t depth; } canvas;
typedef struct file_ppm { char magic[2]; canvas *cs; } file_ppm;
typedef struct { size_t x; size_t y; size_t w; size_t h; } rectangle;
typedef struct { size_t x; size_t y; size_t r; } circle;
typedef struct { size_t x0; size_t y0; size_t x1; size_t y1; } line;

rgb rgb_new(uint8_t r, uint8_t g, uint8_t b);
rgb *rgb_white();
rgb *rgb_blue();
rgb *rgb_red();

canvas *cs_new(size_t width, size_t height, size_t depth);
void cs_free(canvas *cs);

void cs_set(canvas *c, size_t line, size_t col, rgb *color);
void cs_draw_rect(canvas *c, rectangle *r, rgb *color);
void cs_draw_circle(canvas *cs, circle *ci, rgb *color);
void cs_draw_line(canvas *cs, line *li, rgb *color);

void rgb_print(char *string, rgb *c);
void rgb_print_rgb(char *string, uint8_t r, uint8_t g, uint8_t b);
int cs_write_ppm(canvas *cs, char *filepath);

int read_until_space(char *result, FILE *fp);
file_ppm *rgb_read_ppm(char *filepath);
int cs_write_tga(canvas *cs, char *filepath);


