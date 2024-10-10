#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "json.h"

cJSON *json_save(cJSON *conf, char *path) {
	FILE *fd = fopen(path, "w");
	if(!fd) {
		perror("json_save : fopen failed");
		return NULL;
	}
	char *json = cJSON_Print(conf);
	if(!json) {
		fprintf(stderr, "json_save : call to cJSON_Print returned NULL\n");
		return NULL;
	}

	size_t n_write = fwrite(json, strlen(json), 1, fd);
	if(n_write != 1) {
		fprintf(stderr, "json_save : call to fwrite return bad length : %ld != 1\n", 
				n_write);
		return NULL;
	}

	n_write = fwrite("\n", 1, 1, fd);
	if(n_write != 1) {
		fprintf(stderr, "json_save : call to fwrite return bad length : %ld != 1\n", 
				n_write);
		return NULL;
	}

	free(json);
	fclose(fd);

	return conf;
}

cJSON *json_load(const char *path) {
	cJSON *conf;
	char buffer[BUF_SIZE];
	int fd = open(path, O_RDONLY);
	if(!fd) {
		perror("json_load : open error");
		return NULL;
	}

	size_t n_read = read(fd, buffer, BUF_SIZE);
	if(n_read >= BUF_SIZE) {
		fprintf(stderr, "json_load : json file too long (%ld)", n_read);
		close(fd);
		return NULL;
	}
	buffer[n_read] = '\0';

	conf = cJSON_Parse(buffer);

	close(fd);

	return conf;
}


