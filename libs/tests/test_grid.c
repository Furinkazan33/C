#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "macros.h"
#include "grid.h"

int test_grid() {
	grid g;
	
	memset(&g, 0, sizeof(grid));
	g.totalWidth = 1200;
	g.totalHeight = 800;
	g.cellWidth = 200;
	g.cellHeight = 200;
	g.verticalMargin = 10;
	g.horizontalMargin = 10;
	grid_setDimensions(&g);
	//grid_LOG_S(&g);
	
	//puts("");
	memset(&g, 0, sizeof(grid));
	g.totalWidth = 1200;
	g.totalHeight = 800;
	g.cols = 4;
	g.rows = 4;
	g.verticalMargin = 10;
	g.horizontalMargin = 10;
	grid_setDimensions(&g);
	//grid_LOG_S(&g);


	return 0;
}


int main(int argc, char **argv) {
	if(argc != 1) { printf("usage : %s\n", argv[0]); return 1; }

	test_grid();
	


	return 0;
}

