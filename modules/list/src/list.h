#include <stdlib.h>
#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

typedef struct vlist { void *data; struct vlist *next; } vlist;

#define LIST_DO(l, ptr, block) { vlist *_tmp = NULL; for(vlist *ptr = (vlist *) l; ptr; ptr = _tmp) { _tmp = ptr->next; block; } }
#define LIST_MAP(l, f) LIST_DO(l, tmp, f(tmp->data))
#define LIST_MAP1(l, f, arg) LIST_DO(l, tmp, f(tmp->data, arg))

#define LIST_FREE(l, func) func(((vlist *)l)->data); free(l)
#define LIST_FREE_ALL(l, func) LIST_DO(l, ptr, LIST_FREE(ptr, func))

#endif


void *list_new(void *data);
void *list_tail(void *l);
void *list_insert_before(void *l, void *data);
void *list_insert_after(void *l, void *data);

/* get pointer to the nth following element */
void *list_get(void *l, size_t n);

/* forward list pointer and return element */
void *list_next(void *ref);

void *list_copy(void *l, void *(*copy)(void *));
/* compare l1 to l2. l1 can be longer than l2 */
bool list_equal(void *l1, void *l2, bool (*eq)(void *, void *), void **e);


