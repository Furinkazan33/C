#include <inttypes.h>

#define _POSIX_C_SOURCE 200809L

#define any uint8_t

void *mem_map(any *start, int n, int stride, int (*function)(any *));
any *mem_find(any *start, int n, int stride, any *value, int (*cmp)(any *, any*));

