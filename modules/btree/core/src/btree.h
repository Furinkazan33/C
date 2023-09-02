#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

#ifndef BTREE_DATA_TYPE
	#define BTREE_DATA_TYPE void
#endif

typedef struct btree {
	BTREE_DATA_TYPE *data;
	struct btree *left;
	struct btree *right;
} btree;


void btree_run_lr_pre(btree *tree, void (*func)(BTREE_DATA_TYPE *));
void btree_run_lr_inf(btree *tree, void (*func)(BTREE_DATA_TYPE *));
void btree_run_lr_post(btree *tree, void (*func)(BTREE_DATA_TYPE *));
void btree_run_rl_pre(btree *tree, void (*func)(BTREE_DATA_TYPE *));
void btree_run_rl_inf(btree *tree, void (*func)(BTREE_DATA_TYPE *));
void btree_run_rl_post(btree *tree, void (*func)(BTREE_DATA_TYPE *));

BTREE_DATA_TYPE *btree_find_lr_pre(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);
BTREE_DATA_TYPE *btree_find_lr_inf(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);
BTREE_DATA_TYPE *btree_find_lr_post(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);
BTREE_DATA_TYPE *btree_find_rl_pre(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);
BTREE_DATA_TYPE *btree_find_rl_inf(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);
BTREE_DATA_TYPE *btree_find_rl_post(btree *tree, int (*compare)(BTREE_DATA_TYPE *, BTREE_DATA_TYPE *), BTREE_DATA_TYPE *data);

btree *btree_new(BTREE_DATA_TYPE *data, btree *left, btree *right);

void btree_free(btree *tree, void (*free_func)(BTREE_DATA_TYPE *));

