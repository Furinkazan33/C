#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_CLIENTS 2
#define MESS_BUF_LEN 10

/*
 * Check activity on clients sockets and repsond to incoming messages
 * */

/* read and response from 1 client */
int sock_serv_rw_1(int fd, char *buffer, int (*mess_handler)(char *, int)) {
	char tmp[MESS_BUF_LEN];
	char err[256];
	int bytesRead;
	char *found;

	// Read data from client socket
	memset(tmp, 0, MESS_BUF_LEN);
	bytesRead = read(fd, tmp, MESS_BUF_LEN);

	if (bytesRead > 0) {
		//printf("sock_serv_rw_1 : client %d :%s:\n", fd, tmp);
		found = strchr(tmp, CHAR_DELIM);

		/* delim found => buffer is a complete message */
		if(found) {
			if(strlen(buffer) + found - tmp >= MESS_BUF_LEN - 1) {
				fprintf(stderr, "sock_serv_rw_1 : message too long from %d :%s:%s:\n", fd, buffer, tmp);
				memset(buffer, 0, MESS_BUF_LEN);
				memset(tmp, 0, MESS_BUF_LEN);
				return 0;
			}

			strncat(buffer, tmp, found - tmp);
			mess_handler(buffer, fd);
			memset(buffer, 0, MESS_BUF_LEN);

			if(*(found + 1) != '\0') {
				strcpy(buffer, found + 1);
			}
		}
		else {
			if(strlen(buffer) + strlen(tmp) >= MESS_BUF_LEN - 1) {
				fprintf(stderr, "sock_serv_rw_1 : message too long from %d :%s:%s:\n", fd, buffer, tmp);
				memset(buffer, 0, MESS_BUF_LEN);
				memset(tmp, 0, MESS_BUF_LEN);
				return 0;
			}
			strncat(buffer, tmp, strlen(tmp));
		}
	}
	else if (bytesRead == 0) {
		printf("Client %d disconnected\n", fd);
		memset(buffer, 0, MESS_BUF_LEN);
		memset(tmp, 0, MESS_BUF_LEN);
		return 0;
	}
	else {
		sprintf(err, "handle_connections : read %d failed", fd);
		perror(err);
		memset(buffer, 0, MESS_BUF_LEN);
		memset(tmp, 0, MESS_BUF_LEN);
		return 0;
	}

	memset(tmp, 0, MESS_BUF_LEN);

	return 1;
}

/* read and response from all clients */
int sock_serv_rw(struct pollfd *fds, char *buffer, int (*mess_handler)(char *, int)) {
	for (int i = 1; i < MAX_CLIENTS + 1; i++) {
		if(fds[i].fd > 0 && fds[i].revents & POLLIN) {
			if(!sock_serv_rw_1(fds[i].fd, &buffer[i * MESS_BUF_LEN], mess_handler)) {
				close(fds[i].fd);
				fds[i].fd = 0;
				fds[i].events = 0;
			}
		}
	}
	return 1;
}

/* Accept connection and set fds if not NULL, else respond that no socket available */
int sock_serv_accept(int server_socket, struct pollfd *fds) {
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);

	int socket = accept(server_socket, (struct sockaddr*)&clientAddress, &clientAddressLength);

	if (socket >= 0) {
		printf("Client establishing connection: %s:%d [%d]\n",
						inet_ntoa(clientAddress.sin_addr),
						ntohs(clientAddress.sin_port),
						socket);

		if(fds == NULL) {
			write(socket, "MAXCLI", 7);
			close(socket);
			return 0;
		}

		// Add client to the first available client socket
		write(socket, "CONOK;", 7);
		fds->fd = socket;
		fds->events = POLLIN;
	}
	else {
		printf("No client connection\n");
	}

	return 1;
}

int sock_serv_init(struct pollfd *fds) {
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
	serverAddress.sin_port = htons(12345);

	if(bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		perror("Socket binding failed");
		return 0;
	}

	// Listen for connections
	if (listen(serverSocket, 5) < 0) {
		perror("Socket listen failed");
		return 0;
	}

	printf("Server is listening on port 12345\n");

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


