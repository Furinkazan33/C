#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


void conf_print(config *c) {
	for(size_t i = 0; i < c->n; i++) {
		fprintf(stdout, "%s=%s\n", c->properties[i]->name, c->properties[i]->value);
	}
}
void conf_free(config *conf) {
	for(size_t i = 0; i < conf->n; i++) {
		free(conf->properties[i]);
	}
	free(conf->properties);
	free(conf);
}

property *prop_new(line *l, char **found) {
	size_t len;
	property *res;

	if(strlen(l->content) <= 3) {
		*found = NULL;
		return NULL;
	}

	res = (property *)malloc(sizeof(property));
	if(!res) {
		perror("prop_new : alloc failed");
		return NULL;
	}

	(*found) = strchr(l->content, ':');
	if(!found) {
		fprintf(stderr, "prop_new : property assignation not found\n");
		return NULL;
	}
	len = (*found) - l->content;

	if(len >= PROP_NAME_LEN) {
		len = PROP_NAME_LEN - 1;
	}

	strncpy(res->name, l->content, len);
	res->name[len] = '\0';

	len = strlen(l->content) - len;
	if(len >= PROP_VALUE_LEN) {
		len = PROP_VALUE_LEN - 1;
	};
	strncpy(res->value, (*found) + 1, len);
	res->value[len] = '\0';

	return res;
}

config *config_read(const char *path, const char *name) {
	tb *buf;
	config *conf;
	char *found;
	FILE *f = fopen(path, "r");
	if(!f) {
		perror("");
		return NULL;
	}

	buf = tb_new(path, name);
	if(!buf) {
		return NULL;
	}

	if(!tb_load(buf)) {
		return NULL;
	}

	/* new config */
	conf = (config *)malloc(sizeof(config));
	conf->size = CONF_ALLOC;
	conf->properties = (property **) malloc(sizeof(property *) * conf->size);
	conf->n = 0;

	for(size_t i = 0; i < buf->n; i++) {
		conf->properties[conf->n] = prop_new(buf->lines[i], &found);

		if(found) {
			if(!conf->properties[conf->n]) {
				fprintf(stderr, "config_read : property creation error\n");
				return NULL;
			}
			conf->n++;

			/* realloc */
			if(conf->n >= conf->size) {
				conf->size *= 2;
				conf->properties = (property **)realloc(conf->properties, sizeof(property *) * conf->size);
				if(!conf->properties) {
					perror("config_read : properties realloc failed");
					return NULL;
				}
			}
		}
	}

	tb_free(buf);

	return conf;
}


