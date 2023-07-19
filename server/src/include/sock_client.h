#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "frd.h"


int sock_cli_connect(char *adress, int port, int fd, struct sockaddr_in *sa) {
	memset(sa, 0, sizeof(*sa));
	sa->sin_family = AF_INET;
	sa->sin_addr.s_addr = inet_addr(adress);
	sa->sin_port = htons(port);

	if(connect(fd, (struct sockaddr *)sa, sizeof(*sa)) < 0) {
		perror("sock_cli_connect : connection failed");
		return 0;
	}

	printf("sock_cli_connect : Connected to server [%s]:[%d]\n", adress, port);
	return 1;
}

int sock_net_new(int *fd) {
	*fd = socket(AF_INET, SOCK_STREAM, 0);

	if(*fd < 0) {
		perror("sock_net_new : net socket creation failed");
		return 0;
	}
	return 1;
}

int sock_cli_init_select(int fd, fd_set *read_set) {
	// Set up file descriptor sets for select
	FD_ZERO(read_set);
	FD_SET(fd, read_set);

	// Use select to wait for activity on fds
	if (select(fd + 1, read_set, NULL, NULL, NULL) < 0) {
		perror("sock_init_select : select failed");
		return 0;
	}

	return 1;
}

int sock_cli_init(int *fd, char *adress, int port, struct sockaddr_in *sa, fd_set *read_set) {
	if(!sock_net_new(fd)) {
		return 0;
	}

	if(!sock_cli_connect(adress, port, *fd, sa)) {
		return 0;
	}

	if(!sock_cli_init_select(*fd, read_set)) {
		return 0;
	}
	return 1;
}


