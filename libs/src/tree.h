#include <stddef.h> 




typedef struct tree {
	char *data;
	struct tree **children;
} tree;



tree *tree_new(char *data, char *(*datacpy)(const char *), int children_array_init_cap);
void tree_free(tree *t, void (*datafree)(void *));
tree *tree_add_child(tree *t, tree *child);
tree *tree_copy(tree *src, char *(*datacpy)(const char *));
tree *tree_add(tree *dest, tree *src, int (*cmp)(const tree *, const tree *), char *(*datacpy)(const char *));
int tree_map(tree *t, int (*f)(tree *, size_t depth, void *, void *, void *), size_t depth, void *arg1, void *arg2, void *arg3);

/*
	stree = tree of char *
*/
int stree_fprintf(tree *t, size_t depth, void *arg1, void *arg2, void *arg3);
int stree_cmp_data(const tree *d1, const tree *d2);
tree *stree_new(char *data, int children_array_init_cap);
tree *stree_add(tree *dest, tree *src);
tree *str_array_to_tree(char **path, size_t len);
char **str_split(char *str, char delim, size_t *len);
tree *str_path_to_tree(char *path);
tree *filelist_to_tree(char **list, size_t len);
