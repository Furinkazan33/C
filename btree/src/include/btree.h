#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

/*
   node or leaf when left and right are NULL
*/
typedef struct btree{
	void *data; 
	struct btree *left;
	struct btree *right;
} btree;


void btree_run_lr_pre(btree *tree, void (*func)(void *));
void btree_run_lr_inf(btree *tree, void (*func)(void *));
void btree_run_lr_post(btree *tree, void (*func)(void *));
void btree_run_rl_pre(btree *tree, void (*func)(void *));
void btree_run_rl_inf(btree *tree, void (*func)(void *));
void btree_run_rl_post(btree *tree, void (*func)(void *));

void *btree_find_lr_pre(btree *tree, int (*compare)(void *, void *), void *data);
void *btree_find_lr_inf(btree *tree, int (*compare)(void *, void *), void *data);
void *btree_find_lr_post(btree *tree, int (*compare)(void *, void *), void *data);
void *btree_find_rl_pre(btree *tree, int (*compare)(void *, void *), void *data);
void *btree_find_rl_inf(btree *tree, int (*compare)(void *, void *), void *data);
void *btree_find_rl_post(btree *tree, int (*compare)(void *, void *), void *data);

btree *btree_new(void *data, size_t data_size, btree *left, btree *right);

char *btree_to_chars(btree *tree, char *(*data_to_chars)(void *));

void btree_delete(btree *tree, void (*free_data)(void *));
