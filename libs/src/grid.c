#include <assert.h>
#include "grid.h"
#include "macros.h"

void grid_setDimensions(grid *g) {
	assert(g->totalWidth > 0);
	assert(g->totalHeight > 0);
	
	if(g->cols > 0) {
		// compute cellWidth
		assert(g->horizontalMargin >= 0);
		g->cellWidth = (g->totalWidth - g->horizontalMargin) / g->cols - g->horizontalMargin;
	}
	else if(g->cellWidth > 0) {
		// compute cols
		assert(g->horizontalMargin >= 0);
		g->cols = (g->totalWidth - g->horizontalMargin) / (g->cellWidth + g->horizontalMargin);
	}
	
	if(g->rows > 0) {
		// compute cellHeight
		assert(g->verticalMargin >= 0);
		g->cellHeight = (g->totalHeight - g->verticalMargin) / g->rows - g->verticalMargin;
	}
	else if(g->cellHeight > 0) {
		// compute rows
		assert(g->verticalMargin >= 0);
		g->rows = (g->totalHeight - g->verticalMargin) / (g->cellHeight + g->verticalMargin);
	}
}

void grid_LOG_S(grid *g) {
	LOG_S("totalWidth=[%d]\n", g->totalWidth);
	LOG_S("totalHeight=[%d]\n", g->totalHeight);
	LOG_S("cols=[%d]\n", g->cols);
	LOG_S("rows=[%d]\n", g->rows);
	LOG_S("cellWidth=[%d]\n", g->cellWidth);
	LOG_S("cellHeight=[%d]\n", g->cellHeight);
	LOG_S("horizontalMargin=[%d]\n", g->horizontalMargin); 
	LOG_S("verticalMargin=[%d]\n", g->verticalMargin); 
}

