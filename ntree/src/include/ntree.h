#include <stddef.h>

#define _POSIX_C_SOURCE 200809L

/*
   node or leaf when children is NULL
*/

typedef struct ntree {
	void *data; 
	struct ntree **children; /* NULL terminated */
} ntree;


void ntree_run_lr_pre(ntree *ntree, void (*func)(void *));
void ntree_run_lr_post(ntree *ntree, void (*func)(void *));

void *ntree_find(ntree *ntree, int (*compare)(void *, void *), void *data);

ntree *ntree_new(void *data, size_t data_size, ntree **children);

void ntree_delete(ntree *tree, void (*free_data)(void *));
