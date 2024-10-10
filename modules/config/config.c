#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "arrayptr.h"

typedef struct {
	char *name;
	char *value;
} config_var;

ARRAYPTR_DECLARE(config_var, cfg_array);

config_var *cfg_var_new(char *name, char *value) {
	config_var *res;
	MALLOC(res, sizeof(*res), );
	MALLOC(res->name, sizeof(char) * (strlen(name) + 1), free(res););
	MALLOC(res->value, sizeof(char) * (strlen(value) + 1), free(res); free(res->name););
	strcpy(res->name, name);
	strcpy(res->value, value);
	return res;
}

config_var *cfg_var_new_from_string(char *s) {
	char *equal = strchr(s, '=');
	IF_RNULL(!equal, "not a valid line", );

	config_var *res;
	MALLOC(res, sizeof(*res), );

	MALLOC(res->name, sizeof(char) * (equal - s + 1), free(res););
	strncpy(res->name, s, equal - s);
	res->name[equal - s] = '\0';

	MALLOC(res->value, sizeof(char) * (strlen(s) - (equal - s) + 1), free(res->name); free(res););
	strcpy(res->value, equal + 1);

	return res;
}


char *cfg_readline(FILE *f, int *n_read) {
	char *res;
	char c;
	size_t i;
	size_t alloc = 16;

	MALLOC(res, sizeof(char) * (alloc + 1), );

	c = fgetc(f);
	for(i = 0; c != EOF && c !='\0' && c != '\n'; i++) {
		while(i >= alloc) {
			alloc *= 2;
			res = realloc(res, sizeof(char) * (alloc + 1));
			IF_PERROR(!res, "realloc failed", );
		}
		res[i] = c;
		c = fgetc(f);
	}
	if(c == '\n') { res[i++] = c; }
	res[i] = '\0';

	*n_read = i;

	return res;
}


cfg_array *config_load(char *path) {
	config_var *var;
	FILE *fd = fopen(path, "r");
	IF_PERROR(!fd, "fopen failed", );

	cfg_array *res = (cfg_array *) arrayptr_new(10);
	IF_RNULL(!res, "arrayptr_new failed", fclose(fd); );

	int n;
	while(1) {
		char *l = cfg_readline(fd, &n); IF_RNULL(!l, "cfg_readline failed", ARRAYPTR_FREE_F(res, free); fclose(fd););
		if(n == 0) { break; }
		if(l[strlen(l) - 1] == '\n') { l[strlen(l) - 1] = '\0'; }
		var = cfg_var_new_from_string(l);
		if(var) {
			arrayptr_append(res, var);
		}
	}

	return res;
}


void cfg_print(config_var *v) {
	printf("%s=%s\n", v->name, v->value);
}

int main() {
	cfg_array *config = config_load("./config.cfg");

	ARRAYPTR_MAP(config, cfg_print);

	return 0;
}


