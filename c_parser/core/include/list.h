#include <stdlib.h>

typedef struct list {
	void *data;
	struct list *prev;
	struct list *next;
} list;


list *list_new(void *data);

list *list_head(list *l);
list *list_tail(list *l);
list *list_insert_before(list *l, void *data);
list *list_insert_after(list *l, void *data);
/* remove and return next */
void list_pop(list *l);

list *list_copy(list *l, void *(*copy)(void *));
void list_map(list *l, void (*map)(void *));
void list_map2(list *l, void (*map)(void *, void *), void *param);

void list_free(list *l, void (*data_free)(void *));
void list_free_all(list *l, void (*data_free)(void *));


