#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

#ifndef GEO2D_H
#define GEO2D_H
#endif

typedef struct g2d_point {
	float x;
	float y;
} g2d_point;

typedef struct g2d_poly {
	g2d_point *points;
	size_t n;
	g2d_point barycenter; // update with points
	float rotZ; // around barycenter
	bool isCircle;
	bool isSquare;
	float transX;
	float transY;
} g2d_poly;



