#include <stdlib.h>

#define LOG(stream, mess) fprintf(stream, "%s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess);
#define LOGE(mess) fprintf(stderr, "[ERROR] %s:%s:%d:%s\n", __FILE__, __func__, __LINE__, mess);
#define CHECK(var, mess) if(!var) { LOGE(mess); return 1; }

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

