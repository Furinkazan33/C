#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "config.h"

cJSON *config_save(cJSON *conf, char *path) {
	int fd = open(path, O_WRONLY | O_APPEND | O_CREAT);
	if(!fd) {
		perror("config_save : write open failed");
		return NULL;
	}
	char *json = cJSON_Print(conf);
	if(!json) {
		fprintf(stderr, "config_save : call to cJSON_Print returned NULL\n");
		return NULL;
	}

	int n_write = write(fd, json, strlen(json));
	if(n_write != (int)strlen(json)) {
		fprintf(stderr, "config_save : call to write return bad length : %d != %ld\n", 
				n_write, strlen(json));
		return NULL;
	}

	free(json);
	close(fd);

	return conf;
}

cJSON *config_new(const char *path) {
	cJSON *conf;
	char buffer[BUF_SIZE];
	int fd = open(path, O_RDONLY);
	if(!fd) {
		perror("config_new : open error");
		return NULL;
	}

	size_t n_read = read(fd, buffer, BUF_SIZE);
	if(n_read >= BUF_SIZE) {
		fprintf(stderr, "config_new : config file too long (%ld)", n_read);
		close(fd);
		return NULL;
	}
	buffer[n_read] = '\0';

	conf = cJSON_Parse(buffer);

	close(fd);

	return conf;
}


