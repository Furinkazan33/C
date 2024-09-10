#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

/* rgb */
typedef struct { uint8_t r; uint8_t g; uint8_t b; } rgb;
rgb rgb_new(uint8_t r, uint8_t g, uint8_t b);
void rgb_print(char *string, rgb *c);
void rgb_print_rgb(char *string, uint8_t r, uint8_t g, uint8_t b);
rgb *rgb_white();
rgb *rgb_blue();
rgb *rgb_red();

/* helpers for basic 2d shapes */
typedef struct { size_t x; size_t y; size_t w; size_t h; } rectangle;
typedef struct { size_t x; size_t y; size_t r; } circle;
typedef struct { size_t x0; size_t y0; size_t x1; size_t y1; } line;
void circle_print(circle *c);

/* canvas */
typedef struct { rgb *pixels; size_t width; size_t height; size_t depth; } canvas;
canvas *cs_new(size_t width, size_t height, size_t depth);
void cs_free(canvas *cs);
void cs_set(canvas *c, size_t line, size_t col, rgb *color);
void cs_draw_rect(canvas *c, rectangle *r, rgb *color);
void cs_draw_circle(canvas *cs, circle *ci, rgb *color);
void cs_draw_line(canvas *cs, line *li, rgb *color);

/* files */
typedef struct { char path[2048]; char magic[2]; canvas *cs; } ppm;
ppm *ppm_new(char *filepath, canvas *cs); // cs can be NULL
ppm *ppm_read(ppm *file);
ppm *ppm_write(ppm *file);


