#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ntree.h>

#define _POSIX_C_SOURCE 200809L

/*
#define NTREE_DEBUG 1
*/
#ifdef NTREE_DEBUG
	#define D_PRINT(mess) printf("%s - %d : %s\n", __FILE__, __LINE__, mess)
#else 
	#define D_PRINT(mess) {}
#endif

/*
	Runs through the binary tree
	type :
		0 : prefix
		1 : postfix
*/
void __ntree_run(ntree *tree, void (*func)(void *), int type) {
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

void ntree_run_lr_pre(ntree *tree, void (*func)(void *)) {
	__ntree_run(tree, func, 0);
}

void ntree_run_lr_post(ntree *tree, void (*func)(void *)) {
	__ntree_run(tree, func, 0);
}


void *ntree_find(ntree *tree, int (*compare)(void *, void *), void *data) {
	ntree **p;
	void *found;
	
	if(tree == NULL) {
		return NULL;
	}
	
	if(compare(tree->data, data) == 0) {
		D_PRINT("found");
		return tree->data;
	}
	
	p = tree->children;
	
	while(p && *p) {
		found = ntree_find(*p, compare, data);
		
		if(found) {
			D_PRINT("found");
			return found;
		} else {
			D_PRINT("not found");
		}
		p++;
	}
	
	return NULL;
}


ntree *ntree_new(void *data, size_t data_size, ntree **children) {
	ntree *new = (ntree *) malloc(sizeof(ntree));
	new->data = (void *) malloc(data_size);
	memcpy(new->data, data, data_size);
	new->children = children;
	
	return new;
}

void ntree_delete(ntree *tree, void (*free_data)(void *)) {
	ntree **p = tree->children;
	
	while(p && *p) {
		free(*p++);
	}
	free_data(tree->data);
	free(tree);
}
