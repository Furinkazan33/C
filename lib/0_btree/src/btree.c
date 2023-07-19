#include <stdlib.h>
#include <string.h>
#include "btree.h"

#define _POSIX_C_SOURCE 200809L

/*
	Runs through the binary tree
	direction :
		0 : left then right
		1 : right then left
	type :
		0 : prefix
		1 : infix
		2 : postfix
*/
void __btree_run(btree *btree, void (*func)(DATA_TYPE *), int direction, int type) {
	if(btree == NULL) {
		return;
	}

	if(direction == 0) {
		switch(type) {
			case 0:
				func(btree->data);
				__btree_run(btree->left, func, direction, type);
				__btree_run(btree->right, func, direction, type);
				break;
			case 1:
			default:
				__btree_run(btree->left, func, direction, type);
				func(btree->data);
				__btree_run(btree->right, func, direction, type);
				break;
			case 2:
				__btree_run(btree->left, func, direction, type);
				__btree_run(btree->right, func, direction, type);
				func(btree->data);
				break;
		}

	} else {
		switch(type) {
			case 0:
				func(btree->data);
				__btree_run(btree->right, func, direction, type);
				__btree_run(btree->left, func, direction, type);
				break;
			case 1:
			default:
				__btree_run(btree->right, func, direction, type);
				func(btree->data);
				__btree_run(btree->left, func, direction, type);
				break;
			case 2:
				__btree_run(btree->right, func, direction, type);
				__btree_run(btree->left, func, direction, type);
				func(btree->data);
				break;
		}
	}
}

void btree_run_lr_pre(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, 0, 0);
}

void btree_run_lr_inf(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, 0, 1);
}

void btree_run_lr_post(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, 0, 2);
}

void btree_run_rl_pre(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, -1, 0);
}

void btree_run_rl_inf(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, -1, 1);
}

void btree_run_rl_post(btree *btree, void (*func)(DATA_TYPE *)) {
	__btree_run(btree, func, -1, 2);
}

/*
	As __btree_run, but looking for data
*/
DATA_TYPE *__btree_find(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data, int direction, int type) {
	DATA_TYPE *res;

	if(btree == NULL) {
		return NULL;
	}

	if(direction == 0) {
		switch(type) {
			case 0:
				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				res = __btree_find(btree->left, compare, data, direction, type);
				if(res) { return res; }

				return __btree_find(btree->right, compare, data, direction, type);
				break;
			case 1:
			default:
				res = __btree_find(btree->left, compare, data, direction, type);
				if(res) { return res; }

				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				return __btree_find(btree->right, compare, data, direction, type);
				break;
			case 2:
				res = __btree_find(btree->left, compare, data, direction, type);
				if(res) { return res; }

				res = __btree_find(btree->right, compare, data, direction, type);
				if(res) { return res; }

				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				return NULL;
				break;
		}

	} else {
		switch(type) {
			case 0:
				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				res = __btree_find(btree->right, compare, data, direction, type);
				if(res) { return res; }

				return __btree_find(btree->left, compare, data, direction, type);
				break;
			case 1:
			default:
				res = __btree_find(btree->right, compare, data, direction, type);
				if(res) { return res; }

				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				return __btree_find(btree->left, compare, data, direction, type);
				break;
			case 2:
				res = __btree_find(btree->right, compare, data, direction, type);
				if(res) { return res; }

				res = __btree_find(btree->left, compare, data, direction, type);
				if(res) { return res; }

				if(compare(btree->data, data) == 0) {
					return btree->data;
				}
				return NULL;
				break;
		}
	}
}

DATA_TYPE *btree_find_lr_pre(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, 0, 0);
}

DATA_TYPE *btree_find_lr_inf(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, 0, 1);
}

DATA_TYPE *btree_find_lr_post(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, 0, 2);
}

DATA_TYPE *btree_find_rl_pre(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, -1, 0);
}

DATA_TYPE *btree_find_rl_inf(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, -1, 1);
}

DATA_TYPE *btree_find_rl_post(btree *btree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	return __btree_find(btree, compare, data, -1, 2);
}

btree *btree_new(DATA_TYPE *data, btree *left, btree *right) {
	btree *new = (btree *) malloc(sizeof(btree));
	new->data = data;
	new->left = left;
	new->right = right;

	return new;
}

void btree_free(btree *tree, void (*free_func)(DATA_TYPE *)) {
	btree_run_lr_post(tree, free_func);
	free(tree);
}
