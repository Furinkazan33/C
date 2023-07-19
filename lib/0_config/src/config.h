#include <stdio.h>
#include "tb.h"

#define PROP_NAME_LEN 32
#define PROP_VALUE_LEN 128
#define CONF_ALLOC 10

typedef struct property {
	char name[PROP_NAME_LEN];
	char value[PROP_VALUE_LEN];
} property;

typedef struct config {
	size_t size;
	size_t n;
	property **properties;
} config;

config *config_read(const char *path, const char *name);
void conf_free(config *conf);
property *prop_new(line *l, char **found);
void conf_print(config *c);

