#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

#ifndef BTREE_H
#define BTREE_H

#define BTREE_DECLARE(type, name) typedef struct name { \
	type *data; \
	struct name *left; \
	struct name *right; \
} name

BTREE_DECLARE(void, btree);

#define BTREE_FREE(t, f) btree_free(t, (void (*)(void *)) f)
#define BTREE_FIND(t, f, d) btree_find(t, (int (*)(void *, void *))f, d)
#define BTREE_MAP(t, f) btree_map(t, (void (*)(void *))f)
#endif

void *btree_new(void *data, void *left, void *right);
void btree_free(void *t, void (*f)(void *));
void *btree_find(void *tree, int (*compare)(void *, void *), void *data);
void btree_map(void *btree, void (*func)(void *));

