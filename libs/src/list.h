#include <stdlib.h>
#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

typedef struct vlist { void *data; struct vlist *next; } vlist;

#define VA_ARGS(...) , ##__VA_ARGS__


#define LIST_DO(list_var, cur_var, block) { \
	vlist *_tmp = NULL; \
	\
	for(vlist *cur_var = (vlist *) (list_var); cur_var; cur_var = _tmp) { \
		_tmp = cur_var->next; \
		block; \
	} \
}

#define LIST_MAP(list_var, map_fun) { \
	vlist *_tmp = NULL; \
	\
	for(vlist *ptr = (vlist *) (list_var); ptr; ptr = _tmp) { \
		_tmp = ptr->next; \
		(map_fun)(ptr); \
	} \
}

#define LIST_MAP_ARGS(list_var, map_fun, ...) { \
	vlist *_tmp = NULL; \
	\
	for(vlist *ptr = (vlist *) (list_var); ptr; ptr = _tmp) { \
		_tmp = ptr->next; \
		(map_fun)(ptr VA_ARGS(__VA_ARGS__)); \
	} \
}

#define LIST_MAP_DATA(list_var, map_fun) { \
	vlist *_tmp = NULL; \
	\
	for(vlist *ptr = (vlist *) (list_var); ptr; ptr = _tmp) { \
		_tmp = ptr->next; \
		(map_fun)(ptr->data); \
	} \
}

#define LIST_MAP_DATA_ARGS(list_var, map_fun, ...) { \
	vlist *_tmp = NULL; \
	\
	for(vlist *ptr = (vlist *) (list_var); ptr; ptr = _tmp) { \
		_tmp = ptr->next; \
		(map_fun)(ptr->data VA_ARGS(__VA_ARGS__)); \
	} \
}

#define LIST_FREE(list_var, free_data_fun) \
	(free_data_fun)(((vlist *)(list_var))->data); \
	free(list_var); \
	list_var = NULL;

#define LIST_FREE_ALL(list_var, free_data_fun) \
	LIST_DO(list_var, X, LIST_FREE(X, free_data_fun);); \
	list_var = NULL;
#endif


vlist *list_new();
vlist *list_new_from(void *data);

void list_free(vlist *l, void (*f)(void *));
void list_free_all(vlist *l, void (*f)(void *));

vlist *list_tail(vlist *l);
vlist *list_insert_before(vlist *l, void *data);
vlist *list_insert_after(vlist *l, void *data);

vlist *list_pop(vlist **l, size_t n);
vlist *list_get(vlist *l, size_t n);
vlist *list_next(vlist **ref);

vlist *list_copy(vlist *l, void *(*copy)(void *));

vlist *find_prec(vlist *l, void *search, int (*eq)(void *, void *));
vlist *find(vlist *l, void *search, int (*eq)(void *, void *));

void list_remove_next(vlist *l);

/* compare l1 to l2. l1 can be longer than l2 */
bool list_equal(vlist *l1, vlist *l2, bool (*eq)(const void *, const void *), vlist **e);

vlist *array_to_list(char *data, size_t count, size_t item_size);
