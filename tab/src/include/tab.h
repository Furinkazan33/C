#include <stdint.h>

#define _POSIX_C_SOURCE 200809L

/* 
Call p{n} times function p{function} over p{offset} 
incrementing by p{stride}
*/
void tab_sweep(uint8_t *offset, int n, int stride, void (*function)(void *));