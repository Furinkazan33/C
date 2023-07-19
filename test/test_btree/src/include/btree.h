#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

#ifndef DATA_TYPE
	#define DATA_TYPE void
#endif

typedef struct btree {
	DATA_TYPE *data;
	struct btree *left;
	struct btree *right;
} btree;


void btree_run_lr_pre(btree *tree, void (*func)(DATA_TYPE *));
void btree_run_lr_inf(btree *tree, void (*func)(DATA_TYPE *));
void btree_run_lr_post(btree *tree, void (*func)(DATA_TYPE *));
void btree_run_rl_pre(btree *tree, void (*func)(DATA_TYPE *));
void btree_run_rl_inf(btree *tree, void (*func)(DATA_TYPE *));
void btree_run_rl_post(btree *tree, void (*func)(DATA_TYPE *));

DATA_TYPE *btree_find_lr_pre(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);
DATA_TYPE *btree_find_lr_inf(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);
DATA_TYPE *btree_find_lr_post(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);
DATA_TYPE *btree_find_rl_pre(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);
DATA_TYPE *btree_find_rl_inf(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);
DATA_TYPE *btree_find_rl_post(btree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data);

btree *btree_new(DATA_TYPE *data, btree *left, btree *right);

void btree_free(btree *tree, void (*free_func)(DATA_TYPE *));

