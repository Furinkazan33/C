#include <stdio.h>

typedef struct point {
    int x;
    int y;
    int z;
} Point;

typedef struct translation {
    int x;
    int y;
    int z;
} Translation;

typedef struct rotation {
    float rotat_x;
    float rotat_y;
    float rotat_z;
} Rotation;

typedef struct transformation {
    Translation translation;
    Rotation rotation;
} Transformation;

typedef struct triangle {
    Point *a;
    Point *b;
    Point *c;
} Triangle;

typedef struct list {
    Triangle *triangle;
    struct list *next;
} Structure;


void apply(Transformation *t, Structure *s) {
    Triangle *p = s->triangle;

    while(*p != NULL) {
        apply_trans(t->translation, p->triangle);
	apply_rot(t->rotation, p->triangle);
	p = p->next;
    }
}

void apply_trans(Translation *t, Triangle *triangle) {
    Point *aa = (Point *) malloc(sizeof(Point *);
    triangle->a.x += t.x;
    triangle->a.y += t.y;
    triangle->a.z += t.z;
}

void apply_rot(Rotation r, Triangle *triangle) {
    // TODO: rotation
}


int main(void) {


    return 0;
}

