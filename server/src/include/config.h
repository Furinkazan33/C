#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CONF_STR_LEN 32
#define CONF_N 5

static int config_write();
static int config_read(char *path);
static void config_print();

static char CONFIG_names[CONF_N][CONF_STR_LEN] = {
	/* config file path */
	"FILEPATH",

	/* Admin access */
	"FIFO_ADM_RD",
	"FIFO_ADM_WR",

	/* Clients */
	"CL_PORT",
	"CL_MAX",
};
static char CONFIG_values[CONF_N][CONF_STR_LEN] = {
	"",
	"",
	"",
	"",
	"",
};

int config_write() {
	FILE *f = fopen(CONFIG_values[0], "w");

	for(int i = 1; i < CONF_N; i++) {
		fprintf(f, "%s\n", CONFIG_values[i]);
	}
	return 0;
}

int config_read(char *path) {
	char c;
	int i, k;
	FILE *f = fopen(path, "r");
	if(!f) {
		perror("");
		return 1;
	}

	strcpy(CONFIG_values[0], path);

	for(i = 1; i < CONF_N; i++) {
   		c = fgetc(f);

		for(k = 0; c != '\n' && c != '\0' && k < CONF_STR_LEN; k++) {
			CONFIG_values[i][k] = c;
			c = fgetc(f);
		}
		if(k >= CONF_STR_LEN) {
			fprintf(stderr, "Config value too long [%d]", k);
			return 1;
		}
		CONFIG_values[i][k] = '\0';
	}

	return 0;
}

void config_print() {
	for(int i = 0; i < CONF_N; i++) {
		printf("%s=%s\n", CONFIG_names[i], CONFIG_values[i]);
	}
}

