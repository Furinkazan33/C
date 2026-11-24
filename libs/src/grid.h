
typedef struct grid {
	int totalWidth;
	int totalHeight;
	int cols;			// number of
	int rows;			// number of
	int cellWidth;
	int cellHeight;
	int verticalMargin;
	int horizontalMargin;
} grid;


void grid_setDimensions(grid *g);
void grid_LOG_S(grid *g);
