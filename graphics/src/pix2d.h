#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#ifndef PIX2D_H
#define PIX2D_H
#endif

#define PIX2D_T_H 6
#define PIX2D_T_W 6
#define PIX2D_T_S PIX2D_T_H * PIX2D_T_W 

typedef uint8_t rgba[4];

extern rgba rgba_black;
extern rgba rgba_white;
extern rgba rgba_red;
extern rgba rgba_blue;

/* helpers for basic 2d shapes */
typedef struct rectangle { size_t x; size_t y; size_t w; size_t h; } rectangle;
typedef struct circle { size_t x; size_t y; size_t r; } circle;
typedef struct line { size_t x0; size_t y0; size_t x1; size_t y1; } line;

void rgba_draw_rect(uint8_t *pixels, size_t w, rectangle *r, rgba *color);
void rgba_draw_circle(uint8_t *pixels, size_t w, circle *ci, rgba *color);

/* files */
typedef struct { char *path; char magic[2]; size_t depth; size_t w; size_t h; uint8_t *pixels; } ppm;
ppm *ppm_read(ppm *dest);
uint8_t *ppm_write(char *filepath, size_t depth, size_t w, size_t h, uint8_t *pixels);
void ppm_free(ppm *file);


