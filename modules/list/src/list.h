#include <stdlib.h>
#include <sf.h>


typedef struct container {
	void *data;
	struct container *prev;
	struct container *next;
} container;

typedef struct list {
	size_t n;
	sf *sf;
	container *head;
	container *tail;
} list;


list *list_new(sf *sf);

container *list_container_new(container *prev, container *next, void *data);
container *list_insert_after(list *l, container *c, void *data);
container *list_append(list *l, void *data);
container *list_insert_before(list *l, container *c, void *data);
container *list_prepend(list *l, void *data);
container *list_find(list *l, void *data);
container *list_remove(list *l, container *c);
void list_swap(list *l, container *c1, container *c2);

void list_delete(list *l, container *c);
void list_free_container(list *l, container *c);
void list_free(list *l);

void list_print(list *l);

