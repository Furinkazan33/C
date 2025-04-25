#include <stdio.h>
#include <stdlib.h>
#include "macros.h"

typedef float m33[3][3];


void m33_add(m33 *dest, m33 src);
void m33_add_to(m33 *dest, m33 m1, m33 m2);
void m33_print(m33 m);

