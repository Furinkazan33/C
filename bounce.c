#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 200
#define HEIGHT 50
#define G 10 // Gravity
#define DT 1 // Refresh rate in seconds

typedef struct ball {
    int x;
    int y;
    float vx;
    float vy;
} Ball;

typedef struct force {
    int fx;
    int fy;
} Force;



void clearScreen()
{
    system("clear");
}


Force *new_force(int fx, int fy) {
    Force *force = (Force *) malloc(sizeof(Force));
    force->fx = fx;
    force->fy = fy;

    return force;
}

Ball *new_ball(int x, int y) {
    Ball *b = (Ball *) malloc(sizeof(Ball));
    b->x = x;
    b->y = y;
    b->vx = 0;
    b->vy = 0;

    return b;
}

void apply(Force *f, Ball *b) {
    b->x = b->x + b->vx;
    b->y = b->y + b->vy;
    b->vx = b->vx + (float) (DT * f->fx) / 10;
    b->vy = b->vy + (float) (DT * f->fy) / 10;
}

void draw(Ball *b) {
    int i, j;

    for (i = 0; i < HEIGHT; i++) {
    	for (j = 0; j < WIDTH; j++) {
	    if (j == b->x && i == b->y)
	        printf("%c", '0');
	    else
	        printf("%c", ' ');
	}
	printf("\n");
    }
}


int main(void) {
    Ball *b = new_ball(10, 0);
    Force *g = new_force(0, G);
    Force *f = new_force(40, 0);    

    apply(f, b);

    while(b->y < HEIGHT) {
        sleep(0.1);
	apply(g, b);
	clearScreen();
        draw(b);

    }


    return 0;
}



