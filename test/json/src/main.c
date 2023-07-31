#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "ntree.h"
#include "person.h"



int main(int argc, char **argv) {
	if(argc != 2 ) {
		printf("usage : %s json_file\n", argv[0]);
		exit(1);
	}

	int fd = open(argv[1], O_RDONLY);
	char buffer[1024];
	size_t n_read = read(fd, buffer, 1024);
	buffer[n_read] = '\0';
	printf("json read    : %s\n", buffer);

	person *p = person_json_to_person(buffer);
	person_to_json(&buffer[0], p);
	printf("json created : %s\n", buffer);

	person_free(p);
	close(fd);

	return 0;
}


