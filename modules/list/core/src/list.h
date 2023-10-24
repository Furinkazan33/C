#include <stdlib.h>
#include <stdbool.h>

typedef struct list {
	void *data;
	struct list *next;
} list;


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
void list_map_void(list *l, void (*map)(void *));
void list_map_void2(list *l, void (*map)(void *, void *), void *param);
int list_map(list *l, int (*map)(void *));
int list_map2(list *l, int (*map)(void *, void *), void *param);

/* free */
void list_free(list *l, void (*data_free)(void *));
void list_free_all(list *l, void (*data_free)(void *));


