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
void array_map(type_array *a, void (*user_print)(void *));


typedef enum prop_type { INT=0, BOOL, DOUBLE, STRING, ARRAY, OBJ, PTR } prop_type;

typedef struct type_value {
	prop_type type;
	void *value;
} type_value;

typedef struct type_prop {
	char name[50];
	type_value *value;
} type_prop;

type_value *value_new_int(int value);
type_value *value_new_bool(int value);
type_value *value_new_double(double value);
type_value *value_new_char(char value);
type_value *value_new_string(char *value);
type_value *value_new_from_array(type_array *a);
type_value *value_new_obj_from_array(type_array *a);
type_value *value_new_from_varname(char *name);
type_prop *prop_new(char *name, type_value *value);
void prop_print(void *property);
void value_print(void *value);


