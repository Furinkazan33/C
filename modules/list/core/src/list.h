#include <stdlib.h>
#include <stdbool.h>

#ifndef LIST_H
#define LIST_Ha

#define LIST_DECLARE(type, name) typedef struct name { type *data; struct name *next; } name

LIST_DECLARE(void, list);

#define LIST_MAP(l, f) { while(l) { f(l->data); l = l->next; } }
#define LIST_MAP1(l, f, arg) { list *__tmp = l; while(__tmp) { f(__tmp->data, arg); __tmp = __tmp->next; } }

#define LIST_FREE(l, func) func(((list *)l)->data); free(l)
#define LIST_FREE_ALL(l, func) { list *__n = NULL; for(list *p = l; p; p = __n) { __n = p->next; LIST_FREE(p, func); } }
#endif


list *list_new(void *data);
list *list_tail(list *l);
list *list_insert_before(list *l, void *data);
list *list_insert_after(list *l, void *data);

/* get pointer to the nth following element */
list *list_get(list *l, size_t n);

/* forward list pointer and return element */
list *list_next(list **ref);

list *list_copy(list *l, void *(*copy)(void *));
/* compare l1 to l2. l1 can be longer than l2 */
bool list_equal(list *l1, list *l2, bool (*eq)(void *, void *), list **e);


