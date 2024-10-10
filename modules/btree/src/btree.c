#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "btree.h"

#define _POSIX_C_SOURCE 200809L

void *btree_new(void *data, void *left, void *right) {
	btree *res;
	MALLOC(res, sizeof(*res), );
	res->data = data;
	res->left = left;
	res->right = right;
	return res;
}

void btree_free(void *tree, void (*f)(void *)) {
	btree *t = tree;
	if(f) { f(t->data); }
	if(t->left) btree_free(t->left, f);
	if(t->right) btree_free(t->right, f);
	free(t);
}

void btree_map(void *tree, void (*func)(void *)) {
	btree *t = tree;
	func(t->data);
	if(t->left) { btree_map(t->left, func); }
	if(t->right) { btree_map(t->right, func); }
}

void *btree_find(void *tree, int (*compare)(void *, void *), void *data) {
	btree *t = tree;
	void *res;

	if(t == NULL) {
		return NULL;
	}

	if(compare(t->data, data) == 0) {
		return t->data;
	}
	res = btree_find(t->left, compare, data);
	if(res) { return res; }

	return btree_find(t->right, compare, data);
}

