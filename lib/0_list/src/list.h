#include <stdlib.h>

#define LIST_INIT_SIZE 32
#define LIST_REALLOC_COEF 1.5

typedef struct list_sf {
	void (*free)(void *);
	int (*cmp)(void *, void *);
	void (*print)(void *);
} list_sf;

typedef struct list {
	int sorted;
	size_t max_n; 		/* init with LIST_INIT_SIZE */
	size_t n;			/* init with 0 */
	list_sf *f;
	void **data;
} list;


list *list_new_sorted(list_sf *sf);
list *list_new(list_sf *sf);

int list_add(list *l, void *c);

int list_idx(list *l, void *search);
void *list_find(list *l, void *search);

void list_remove(list *l, size_t idx);
void list_remove_keep_nulls(list *l, size_t idx);

void list_free(list *l);

void list_print(list *l);

