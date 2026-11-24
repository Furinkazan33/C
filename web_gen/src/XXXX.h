#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "array.h"

#ifndef WG_H
#define WG_H

#define TAB(res, n) FOR(i, 0, n, strcat(res, "\t"););

typedef struct data_source {
	int id;
	char *name;
	void *data;
} data_source;

typedef struct app {
	int id;
	char *type;
	char *name;
	struct app *parent;
	data_source *src;
	array *children;
} app;


char *gen(app *a);
char *gen_type(app *a, size_t tabs);
char *gen_all(app *a, size_t tabs);
char *gen_table(app *a, size_t tabs);


#endif



