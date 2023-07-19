#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include "frd.h"

#define MAX_CLIENTS 2

/* Accept connection and set fds if not NULL, else respond that no socket available */
int sock_serv_accept(struct pollfd *fds) {
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
		for(int i = 1; i < MAX_CLIENTS + 1; i++) {
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

int sock_serv_init(struct pollfd *fds, int port) {
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
	memset(fds, 0, sizeof(*fds) * (MAX_CLIENTS + 1));
	fds->fd = serverSocket;
	fds->events = POLLIN;

	return 1;
}

void sock_serv_close(struct pollfd *fds) {
	for (int i = 0; i < MAX_CLIENTS + 1; i++) {
		if (fds[i].fd > 0) {
			close(fds[i].fd);
			fds[i].fd = 0;
		}
	}
}


