#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

#ifndef NTREE_DATA_TYPE
  #define NTREE_DATA_TYPE void
#endif

#define NTREE_ALLOC 128

typedef struct ntree {
	NTREE_DATA_TYPE *data;
	struct ntree **children;
	size_t size;
	size_t n;
} ntree;

/* run func on root's data and then on children's data */
void ntree_run_lr_pre(ntree *ntree, void (*func)(NTREE_DATA_TYPE *));

/* run func on children's data and then on root's data */
void ntree_run_lr_post(ntree *ntree, void (*func)(NTREE_DATA_TYPE *));

NTREE_DATA_TYPE *ntree_find(ntree *ntree, int (*compare)(NTREE_DATA_TYPE *, NTREE_DATA_TYPE *), NTREE_DATA_TYPE *data);

ntree *ntree_alloc(ntree *tree);

int ntree_add_child(ntree *tree, ntree *child);

void ntree_free(ntree *tree, void (*free_data)(NTREE_DATA_TYPE *));

void ntree_to_s(char *dest, ntree *tree, void (*to_s)(char *, NTREE_DATA_TYPE *));

