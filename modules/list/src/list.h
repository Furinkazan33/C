#include <stdlib.h>
#include <stdbool.h>

#ifndef LIST_H
#define LIST_H

#define LIST_DECLARE(type, name) typedef struct name { type *data; struct name *next; } name
LIST_DECLARE(void, vlist);

#define LIST_MAP(l, f) { vlist *tmp = (vlist *) l; while(tmp) { f(tmp->data); tmp = tmp->next; } }
#define LIST_MAP1(l, f, arg) { vlist *tmp = (vlist *) l; while(tmp) { f(tmp->data, arg); tmp = tmp->next; } }

#define LIST_FREE(l, func) func(((vlist *)l)->data); free(l)
#define LIST_FREE_ALL(l, func) { vlist *tmp = NULL; for(vlist *p = (vlist *) l; p; p = tmp) { tmp = p->next; LIST_FREE(p, func); } }

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


