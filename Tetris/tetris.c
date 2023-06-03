#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define _POSIX_C_SOURCE 200809L

#define GRID_WIDTH 10
#define GRID_HEIGHT 20
#define MAX_SHAPE_WIDTH 4
#define MAX_SHAPE_HEIGHT 4
#define N_SHAPES 6

enum Type { 
	KING = 0, QUEEN, KNIGHT1, KNIGHT2, ROOK, PAWN
};

const int SHAPES[N_SHAPES][MAX_SHAPE_HEIGHT][MAX_SHAPE_WIDTH] = 
{ 
	{ 
		{1, 1, 1, 0} , 
		{1, 1, 1, 0} , 
		{1, 1, 1, 0} ,
		{0, 0, 0, 0} 
	},
	{ 
		{1, 1, 1, 1} , 
		{0, 0, 0, 0} , 
		{0, 0, 0, 0} ,
		{0, 0, 0, 0} 
	},
	{ 
		{1, 1, 1, 1} , 
		{0, 0, 0, 0} , 
		{0, 0, 0, 0} ,
		{0, 0, 0, 0} 
	},
	{ 
		{1, 1, 1, 1} , 
		{0, 0, 0, 0} , 
		{0, 0, 0, 0} ,
		{0, 0, 0, 0} 
	},
	{ 
		{1, 1, 1, 1} , 
		{0, 0, 0, 0} , 
		{0, 0, 0, 0} ,
		{0, 0, 0, 0} 
	},
	{ 
		{1, 1, 1, 1} , 
		{0, 0, 0, 0} , 
		{0, 0, 0, 0} ,
		{0, 0, 0, 0} 
	},
};


typedef struct {
	enum Type type;
	int decal_x;
	int decal_y;
	int blocs[MAX_SHAPE_HEIGHT][MAX_SHAPE_WIDTH];
	int rotation; //0, 1, 2, 3
} Shape;

Shape current_shape;
int grid[GRID_HEIGHT][GRID_WIDTH];
int to_remove[GRID_HEIGHT];

void shape_init(enum Type type, const int static_blocs[MAX_SHAPE_HEIGHT][MAX_SHAPE_WIDTH]) {
	int i, j;
	
	current_shape.type = type;
	current_shape.decal_x = 0;
	current_shape.decal_y = 0;
	current_shape.rotation = 0;
		
	for(i = 0; i < MAX_SHAPE_HEIGHT; i++) {
		for(j = 0; j < MAX_SHAPE_WIDTH; j++) {
			current_shape.blocs[i][j] = static_blocs[i][j];
		}
	}
}


int inside_current_shape(int i, int j) {
	int x_max = current_shape.decal_x + MAX_SHAPE_WIDTH;
	int y_max = current_shape.decal_y + MAX_SHAPE_HEIGHT;
	
	// Relative to current_shape
	i -= current_shape.decal_y;
	j -= current_shape.decal_x;
		
	if(i < 0 || j < 0 || j > x_max || i > y_max || SHAPES[current_shape.type][i][j] == 0) {
		return 0;
	}
		
	return 1;
}

int __line_is_full(int l) {
	int c;
	
	for (c = 0; c < GRID_WIDTH; c++) {
		if(grid[l][c] == 0) {
			return 0;
		} 
	}
	return 1;
}

void grid_get_to_remove() {
	int l, k;
	
	for (l = 0, k = 0; l < GRID_HEIGHT; l++) {
		if(__line_is_full(l)) {
			to_remove[k] = l;
			k++;
		}
	}
	to_remove[k] = -1;
}

void __grid_remove_line(int l) {
	int i, j;
	
	// Suppresion de la ligne
	for(i = l; i > 0; i--) {
		for(j = 0; j < GRID_WIDTH; j++) {
			grid[i][j] = grid[i - 1][j];
		}
	}
	// Première ligne vide suite à suoppression
	for(j = 0; j < GRID_WIDTH; j++) {
		grid[0][j] = 0;
	}
}

void grid_remove_lines() {
	int i;
	
	for(i = 0; to_remove[i] != -1; i++) {
		__grid_remove_line(to_remove[i]);
	}
}



int is_laying() {
	if(current_shape.decal_y == GRID_HEIGHT) {
		return 1;
	}
	return 0;
}

void get_new_shape() {
	int rnd = rand() % N_SHAPES;
	shape_init(rnd, SHAPES[rnd]);
}

void update_current_shape(int x, int y, int rotation) {
	if(is_laying()) {
		get_new_shape();
		return;
	}
	current_shape.decal_x += x;
	current_shape.decal_y += y;
	current_shape.rotation = (current_shape.rotation + rotation) % 4;
}

void update_grid() {
	int i, j;
	
	for(i = 0; i < GRID_HEIGHT; i++) {
		for(j = 0; j < GRID_WIDTH; j++) {
			if(inside_current_shape(i, j)) {
				grid[i][j] = 1;
			} else {
				grid[i][j] = 0;
			}
		}
	}
	grid_get_to_remove();
	grid_remove_lines();
}

void draw_grid() {
	int i, j, k;
	
	for(i = 0; i < GRID_HEIGHT; i++) {
		for(j = 0; j < GRID_WIDTH; j++) {
			if(grid[i][j] == 1) {
				printf("%c", 'X');
			} else {
				printf("%c", '0');
			}
		}
		printf("\n");
	}
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void setup() {
	get_new_shape();
	
}

void update() {
	update_current_shape(0, 0, 0);
	update_grid();
	draw_grid();
}

int main() {
	
	
	setup();
	
	while(1) {
		clearScreen();
		update();
		sleep(1);
	}
	
	return 0;
}