#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ntree.h"

#define _POSIX_C_SOURCE 200809L

/*
	Runs through the tree
	type :
		0 : prefix (data first)
		1 : postfix (children first)
*/
void __ntree_run(ntree *tree, void (*func)(DATA_TYPE *), int type) {
	ntree **p;

	if(tree == NULL) {
		return;
	}

	p = tree->children;

	switch(type) {
		case 0:
			func(tree->data);

			while(p && *p) {
				__ntree_run(*p++, func, type);
			}

			break;

		case 1:
		default:
			while(p && *p) {
				__ntree_run(*p++, func, type);
			}

			func(tree->data);

			break;
	}
}

void ntree_run_lr_pre(ntree *tree, void (*func)(DATA_TYPE *)) {
	__ntree_run(tree, func, 0);
}

void ntree_run_lr_post(ntree *tree, void (*func)(DATA_TYPE *)) {
	__ntree_run(tree, func, 0);
}


DATA_TYPE *ntree_find(ntree *tree, int (*compare)(DATA_TYPE *, DATA_TYPE *), DATA_TYPE *data) {
	ntree **p;
	DATA_TYPE *found;

	if(tree == NULL) {
		return NULL;
	}

	if(!compare(tree->data, data)) {
		return tree->data;
	}

	p = tree->children;

	while(p && *p) {
		found = ntree_find(*p, compare, data);

		if(found) {
			return found;
		}
		p++;
	}

	return NULL;
}

ntree *ntree_alloc(ntree *root) {
	ntree *res = root;
	ntree **children;

	/* alloc */
	if(res == NULL) {
		res = (ntree *)malloc(sizeof(ntree));
		if(!res) {
			perror("ntree_add : root alloc failed");
			return NULL;
		}
		res->children = (ntree **)malloc(sizeof(ntree *) * NTREE_ALLOC);
		if(!res->children) {
			free(res);
			perror("ntree_add : children alloc failed");
			return NULL;
		}
		res->size = NTREE_ALLOC;
		res->n = 0;
		res->data = NULL;
	}

	/* realloc */
	else if(res->n >= res->size) {
		children = realloc(res->children, res->size * 2);
		if(!children) {
			perror("ntree_add : realloc failed");
			return NULL;
		}
		res->children = children;
		res->size *= 2;
	}
	return res;
}

int ntree_add_child(ntree *tree, ntree *child) {
	ntree *res = ntree_alloc(tree);

	if(!res) {
		return 1;
	}

	tree->children[tree->n++] = child;

	return 0;
}

void ntree_free(ntree *tree, void (*free_data)(DATA_TYPE *)) {
	ntree **p = tree->children;

	while(p && *p) {
		ntree_free(*p, free_data);
		p++;
	}
	free_data(tree->data);
	free(tree);
}

