#include <tab.h>

#define _POSIX_C_SOURCE 200809L

/* 
Call {n} times function {function} over {offset} 
incrementing by {stride}
*/
void tab_sweep(uint8_t *offset, int n, int stride, void (*function)(void *)) {
	int i;
	
	for(i = 0; i < n; i++) {
		function(offset);
		offset += stride;
	}
}
