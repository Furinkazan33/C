#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

int sock_connect_to_serv(char *adress, int port, int fd, struct sockaddr_in *sa) {
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

int sock_select(int fd, fd_set *read_set) {
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


/* Accept connection and set fds if not NULL, else respond that no socket available */
int sock_accept(struct pollfd *fds, int max_clients) {
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);
	struct pollfd *free_fds;
	int socket = accept(fds->fd, (struct sockaddr*)&clientAddress, &clientAddressLength);

	if (socket >= 0) {
		printf("Client establishing connection: %s:%d [%d]\n",
						inet_ntoa(clientAddress.sin_addr),
						ntohs(clientAddress.sin_port),
						socket);

		/* next avalaible fds */
		free_fds = NULL;
		for(int i = 1; i < max_clients + 1; i++) {
			if(fds[i].fd == 0) {
				free_fds = &fds[i];
			}
		}
		if(free_fds == NULL) {
			write(socket, "MAXLI", 6);
			close(socket);
			return 0;
		}

		// Add client to the first available client socket
		write(socket, "CONOK", 6);
		free_fds->fd = socket;
		free_fds->events = POLLIN;
	}
	else {
		printf("No client connection\n");
	}

	return 1;
}

int sock_init_poll(struct pollfd *fds, int port, int max_clients) {
	int serverSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		perror("Socket creation failed");
		return 0;
	}

	// Bind socket
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);

	if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		perror("Socket binding failed");
		return 0;
	}

	// Listen for connections
	if (listen(serverSocket, 5) < 0) {
		perror("Socket listen failed");
		return 0;
	}

	printf("Server is listening on port %d\n", port);

	// Set up pollfd structure for server socket
	memset(fds, 0, sizeof(*fds) * (max_clients + 1));
	fds->fd = serverSocket;
	fds->events = POLLIN;

	return 1;
}

void sock_close_poll(struct pollfd *fds, int max_clients) {
	for (int i = 0; i < max_clients + 1; i++) {
		if (fds[i].fd > 0) {
			close(fds[i].fd);
			fds[i].fd = 0;
		}
	}
}


