#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "config.h"

int config_save(cJSON *conf, char *path) {
	int fd = open(path, O_WRONLY);
	if(!fd) {
		perror("config_save : write open failed");
		return 1;
	}
	char *json = cJSON_Print(conf);
	write(fd, json, strlen(json));

	free(json);
	close(fd);

	return 0;
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
		fprintf(stderr, "config_new : confi file too long (%ld)", n_read);
		close(fd);
		return NULL;
	}
	buffer[n_read] = '\0';

	conf = cJSON_Parse(buffer);

	close(fd);

	return conf;
}


