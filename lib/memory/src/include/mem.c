#include <stdlib.h>
#include "mem.h"

#define _POSIX_C_SOURCE 200809L

void *mem_map(any *start, int n, int stride, int (*function)(any *)) {
	any *p = start;

	for(int i = 0; i < n; i++) {
		if(!function(p)) {
			return p;
		}
		p += stride;
	}
	return NULL;
}

any *mem_find(any *start, int n, int stride, any *value, int (*cmp)(any *, any*)) {
	any *p = start;

	for(int i = 0; i < n; i++) {
		if(!cmp(value, p)) {
			return p;
		}
		p += stride;
	}
	return NULL;
}

