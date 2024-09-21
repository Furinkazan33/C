#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <helpers.h>



int main() {
	uint8_t src[4][4] = { 
		{ 0, 0, 0, 255 }, 
		{ 0, 0, 255, 255 }, 
		{ 0, 255, 0, 255 }, 
		{ 255, 0, 0, 255 }, 
	};
	uint8_t dest[4][3];

	FOR(i, 0, 4, memcpy(&dest[i], &src[i], 3 * sizeof(uint8_t));)
	FOR(i, 0, 4, printf("%d\t%d\t%d\t%d\n", src[i][0], src[i][1], src[i][2], src[i][3]);)
	FOR(i, 0, 4, printf("%d\t%d\t%d\n", dest[i][0], dest[i][1], dest[i][2]);)
}

