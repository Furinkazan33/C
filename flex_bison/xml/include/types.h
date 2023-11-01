#include <stdlib.h>
#include <stdio.h>

#define ARRAY_INIT_SIZE 10

typedef struct type_array {
	size_t size;
	size_t n;
	void **data;
} type_array;

type_array *array_new();
type_array *array_new_from(void *e);
type_array *array_add(type_array *a, void *e);
void array_map(type_array *a, void (*f)(void *));


typedef enum balise_type { T_ATOM=0, T_NORMAL } balise_type;

typedef struct type_balise {
	balise_type type;
	char name[50];
	type_array *attributes;
	void *data; //char * or type_array *
} type_balise;


type_balise *balise_new_atom(char *name, type_array *attributes, char *data);
type_balise *balise_new(char *name, type_array *attributes, type_array *data);

void balise_print(void *balise);

typedef struct type_attribute {
	char name[50];
	char content[256];
} type_attribute;

type_attribute *attribute_new(char *name, char *content);
void attribute_print(void *attribute);


