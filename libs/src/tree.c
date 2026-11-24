#include "tree.h"
#include "arrayh.h"
#include "macros.h"


tree *tree_new(char *data, char *(*datacpy)(const char *), int children_array_init_cap) {
	tree *res;
	MALLOC(res, sizeof(*res), return NULL, );
	
	if(data && datacpy) {
		res->data = datacpy(data);
		if(!res->data) {
			LOG_E("failed to copy data\n");
			return NULL;
		}
	}
	else {
		res->data = data;
	}
	
	if(children_array_init_cap > 0) {
		ARRAYH_NEW(res->children, children_array_init_cap, LOG_E("failed to create children\n"); return NULL, );
	}
	else {
		res->children = NULL;
	}
	
	return res;
}

void tree_free(tree *t, void (*datafree)(void *)) {
	for(size_t i = 0; t->children && i < ARRAYH_CNT(t->children); i++) {
		tree_free(t->children[i], datafree);
	}

	if(datafree && t->data) {
		datafree(t->data);
	}
	
	free(t);
}

tree *tree_add_child(tree *t, tree *child) {
	if(!t->children) {
		ARRAYH_NEW(t->children, 10, LOG_E("failed to create children\n"); return NULL, );
	}
	ARRAYH_ADD(t->children, child, LOG_E("failed to add child\n"); return NULL);
	
	return t;
}

tree *tree_copy(tree *src, char *(*datacpy)(const char *)) {
	char *data = NULL;
	
	if(src->data && datacpy) {
		data = datacpy(src->data);
		if(!data) {
			LOG_E("failed to copy data\n");
			return NULL;
		}
	}

	tree *res = NULL;
	if(!src->children) {
		res = tree_new(data, datacpy, 0);
	}
	else {
		res = tree_new(data, datacpy, ARRAYH_CNT(src->children));
	}
	if(!res) {
		LOG_E("failed to create tree\n");
		return NULL;
	}
	
	for(size_t c = 0; src->children && c < ARRAYH_CNT(src->children); c++) {
		tree *child = tree_copy(src->children[c], datacpy);
		if(!child) {
			LOG_E("failed to create child\n");
			return NULL;
		}
		ARRAYH_ADD(res->children, child, LOG_E("failed to add child copy\n"); return NULL);
	}

	return res;
}

tree *tree_add(tree *dest, tree *src, int (*cmp)(const tree *, const tree *), char *(*datacpy)(const char *)) {
	if(cmp(dest, src) != 0) {
		LOG_E("not same root\n");
		return NULL;
	}
	
	/* 
		comparing children
	*/
	if(src->children && !dest->children) {
		ARRAYH_NEW(dest->children, ARRAYH_CNT(src->children), LOG_E("failed to create children\n"); return NULL, );
	}

	for(size_t i = 0; src->children && i < ARRAYH_CNT(src->children); i++) {
		tree *found;
		ARRAYH_FIND(dest->children, src->children[i], cmp, found);

		// found => reccursive call on children
		if(found) {
			#ifdef DEBUG
				LOG_S("found");
			#endif
			
			tree_add(found, src->children[i], cmp, datacpy);
		}
		// not found => adding a copy
		else {
			#ifdef DEBUG
				LOG_S("not found");
			#endif
			
			tree *copy = tree_copy(src->children[i], datacpy);
			if(!copy) {
				LOG_E("tree_copy failed\n");
				return NULL;
			}
			
			ARRAYH_ADD(dest->children, copy, LOG_E("failed to add children\n"); return NULL);
		}
	}
	
	return dest;
}

int tree_map(tree *t, int (*f)(tree *, size_t depth, void *, void *, void *), size_t depth, void *arg1, void *arg2, void *arg3) {
	int ret = f(t, depth, arg1, arg2, arg3); if(ret < 0) return ret;

	for(size_t i = 0; t->children != NULL && i < ARRAYH_CNT(t->children); i++) {
		ret = tree_map(t->children[i], f, depth + 1, arg1, arg2, arg3); if(ret < 0) return ret;
	}
	
	return 0;
}


/*
	stree = tree of char *
*/
int stree_fprintf(tree *t, size_t depth, void *arg1, void *arg2, void *arg3) {
	(void) arg2;
	(void) arg3;
	int ret = 0;
	
	while(depth-- > 0) {
		ret = fprintf((FILE *) arg1, "  "); if(ret < 0) return ret;
	}
	if(t->data) {
		ret = fprintf((FILE *) arg1, "[%s]\n", (char *) t->data); if(ret < 0) return ret;
	}
	else {
		ret = fprintf((FILE *) arg1, "[<NULL>]\n"); if(ret < 0) return ret;
	}
	
	return 0;
}

char *_strdup(const char *src) {
	char *res;
	MALLOC(res, sizeof(char) * (strlen(src) + 1), return NULL, );
	strcpy(res, src);
	return res;
}

int stree_cmp_data(const tree *d1, const tree *d2) {
	if(!d1->data && d2->data) {
		return -1;
	}
	if(d1->data && !d2->data) {
		return 1;
	}
	if(!d1->data && !d2->data) {
		return 0;
	}
	return strcmp(d1->data, d2->data);
}

tree *stree_new(char *data, int children_array_init_cap) {
	return tree_new(data, _strdup, children_array_init_cap);
}

tree *stree_add(tree *dest, tree *src) {
	return tree_add(dest, src, stree_cmp_data, _strdup);
}

// path do not contains //
tree *str_array_to_tree(char **path, size_t len) {
	tree *res = stree_new(*path, 1);
	if(!res) {
		LOG_E("failed to create tree");
		return NULL;
	}

	if(len > 1) {
		tree *child = str_array_to_tree(path + 1, len - 1); if(!child) { return NULL; }
		
		if(!tree_add_child(res, child)) {
			return NULL;
		}
	}
	
	return res;
}

char **str_split(char *str, char delim, size_t *len) {
	#ifdef DEBUG
		LOG_S("start");
	#endif
	
	char **res;
	size_t cap = 10;
	MALLOC(res, sizeof(*res) * cap, return NULL, );
	*len = 0;
	
	char *slash;
	
	while((slash = strchr(str, delim)) || strlen(str) > 0) {
		char *filename;
		
		if(slash) {
			 ARRAY_REALLOC(res, (*len), cap, return NULL);
			 
			if(slash > str) {
				MALLOC(filename, sizeof(char) * (slash - str + 1), return NULL, );
				strncpy(filename, str, slash - str);
				filename[slash - str] = 0;
				res[(*len)++] = filename;
			}
			else {
				res[(*len)++] = NULL;
			}

			str = slash + 1;
		} 
		else {
			if(strlen(str) > 0) {
				ARRAY_REALLOC(res, (*len), cap, return NULL);
				
				MALLOC(filename, sizeof(char) * (strlen(str) + 1), return NULL, );
				strcpy(filename, str);
				res[(*len)++] = filename;
				
				str += strlen(str);
			}
		}
	}
	
	#ifdef DEBUG
		LOG_S("end");
	#endif
	
	return res;
}

tree *str_path_to_tree(char *path) {
	#ifdef DEBUG
		LOG_S("start");
	#endif
	
	size_t len;
	char **a1 = str_split(path, '/', &len); if(!a1) { return NULL; }

	tree *res = str_array_to_tree(a1, len); if(!res) { return NULL; }

	ARRAY_FREE_F(a1, len, free);
	
	#ifdef DEBUG
		LOG_S("end");
	#endif
	
	return res;
}

tree *filelist_to_tree(char **list, size_t len) {
	#ifdef DEBUG
		LOG_S("start");
	#endif
	
	tree *res = str_path_to_tree(list[0]);
	
	#ifdef DEBUG
		tree_map(res, stree_fprintf, 0, stdout, NULL, NULL);
	#endif
	
	
	for(size_t i = 1; i < len; i++) {
		tree *t = str_path_to_tree(list[i]);
		
		#ifdef DEBUG
			tree_map(t, stree_fprintf, 0, stdout, NULL, NULL);
		#endif
		
		stree_add(res, t);

		tree_free(t, free);
	}
	
	#ifdef DEBUG
		LOG_S("end");
	#endif
	
	return res;
}

