#include <stdlib.h>

#define ARRAY_INIT_SIZE 10

typedef struct type_array {
	size_t size;
	size_t n;
	void **data;
} type_array;

type_array *array_new();
type_array *array_new_from(void *e);
type_array *array_add(type_array *a, void *e);
type_array *array_add_first(type_array *a, void *p);
void array_map(type_array *a, void (*user_print)(void *));

void line_print(type_array *line);
void lines_print(type_array *lines);

