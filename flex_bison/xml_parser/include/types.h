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
	void *data; //char * or type_array *
} type_balise;


type_balise *balise_new_atom(char *name, char *data);
type_balise *balise_new(char *name, type_array *data);

void balise_print(void *balise);


