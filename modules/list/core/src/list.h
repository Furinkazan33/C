#include <stdlib.h>


typedef struct container {
	void *data;
	struct container *prev;
	struct container *next;
} container;

typedef struct list {
	size_t n;
	int (*cmp)(void *, void *);
	void (*free)(void *);
	void (*write)(void *, FILE *);
	container *head;
	container *tail;
} list;


list *list_new();
void list_set_cmp(list *l, int (*cmp)(void *, void *));
void list_set_free(list *l, void (*free)(void *));
void list_set_write(list *l, void (*write)(void *, FILE *));

list *list_insert_before(list *l, container *c, void *data);
list *list_insert_after(list *l, container *c, void *data);
list *list_append(list *l, void *data);
list *list_prepend(list *l, void *data);
void list_swap(list *l, container *c1, container *c2);

container *list_find(list *l, void *data);
container *list_pop(list *l, container *c);

/* remove and free container */
void list_delete(list *l, container *c);

void list_free_container(list *l, container *c);
void list_free(list *l);

void list_write(list *l, FILE *file);

