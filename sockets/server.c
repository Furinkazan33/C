#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define MAX_CLIENTS 2
#define BUF_LEN 1024
#define MAX_READ_ERRORS 20

/*
 * Check activity on clients sockets and repsond to incoming messages
 * */
int handle_connections(struct pollfd *fds) {
	char buffer[BUF_LEN];
	int bytesRead, errors = 0;

	for (int i = 1; i < MAX_CLIENTS + 1; i++) {
		if ((fds+i)->fd > 0 && (fds+i)->revents & POLLIN) {

			// Read data from client socket
			memset(buffer, 0, sizeof(buffer));
			bytesRead = read((fds+i)->fd, buffer, sizeof(buffer));

			if (bytesRead > 0) {
				printf("Received from client: %s\n", buffer);

				// Process client request
				// ...

				strcat(buffer, " => ok");
				write((fds+i)->fd, buffer, strlen(buffer));
			}
			else if (bytesRead == 0) {
				printf("Client disconnected [%d]\n", (fds+i)->fd);
				close((fds+i)->fd);
				(fds+i)->fd = 0;
				(fds+i)->events = 0;
			}
			else {
				sprintf(buffer, "handle_connections : read failed on [%d]", (fds+i)->fd);
				perror(buffer);
				close((fds+i)->fd);
				(fds+i)->fd = 0;
				(fds+i)->events = 0;
				errors++;
			}
		}
	}
	return errors;
}

int handle_new_connections(struct pollfd *fds) {
	char buffer[BUF_LEN];
	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);

	/* Accept on server socket (fds[0]) */
	int clientSocket = accept(fds->fd, (struct sockaddr*)&clientAddress, &clientAddressLength);

	if (clientSocket >= 0) {
		printf("Client establishing connection: %s:%d [%d]\n",
						inet_ntoa(clientAddress.sin_addr),
						ntohs(clientAddress.sin_port),
						clientSocket);

		// Add client to the first available socket
		int i;
		for (i = 1; i < MAX_CLIENTS + 1; i++) {
			if ((fds+i)->fd == 0) {
				(fds+i)->fd = clientSocket;
				(fds+i)->events = POLLIN;
				break;
			}
		}

		// Check if maximum number of clients reached
		if (i == MAX_CLIENTS + 1) {
			sprintf(buffer, "Maximum number of clients reached [%d], closing connection", MAX_CLIENTS);
			fprintf(stderr, "%s\n", buffer);
			write(clientSocket, buffer, strlen(buffer));
			close(clientSocket);
		}
		else {
			strcpy(buffer, "Connection ok");
			write((fds+i)->fd, buffer, strlen(buffer));
		}
	}
	return clientSocket;
}

int init_server(struct pollfd *fds) {
	int ret, serverSocket;

	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket < 0) {
		perror("Socket creation failed");
		return serverSocket;
	}

	// Bind socket
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(12345);

	ret = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	if(ret < 0) {
		perror("Socket binding failed");
		return ret;
	}

	// Listen for connections
	ret = listen(serverSocket, 5);
	if (ret < 0) {
		perror("Socket listen failed");
		return ret;
	}

	printf("Server is listening on port 12345\n");

	// Set up pollfd structure for server socket
	memset(fds, 0, sizeof(*fds) * (MAX_CLIENTS + 1));
	fds->fd = serverSocket;
	fds->events = POLLIN;

	return serverSocket;
}

int main() {
	int read_errors;

	/* +1 for server socket */
	struct pollfd fds[MAX_CLIENTS + 1];

	if(init_server(&fds[0]) < 0) {
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Use poll to wait for activity on sockets
		if (poll(fds, MAX_CLIENTS + 1, -1) < 0) {
			perror("Poll failed");
			exit(EXIT_FAILURE);
		}

		// if server socket has activity, try to accept new connections
		if (fds[0].revents & POLLIN) {
			if(handle_new_connections(&fds[0]) < 0) {
				exit(EXIT_FAILURE);
			}
		}

		read_errors = handle_connections(&fds[0]);

		if(read_errors > 0) {
			fprintf(stderr, "main : [%d] reading errors\n", read_errors);

			if(read_errors > MAX_READ_ERRORS) {
				fprintf(stderr, "main : Too many read errors. Exiting.\n");
				exit(EXIT_FAILURE);
			}
		}

		/* TODO:
		 * break on specific admin message or SIGINT
		 * */
	}

	// Close server socket
	close(fds[0].fd);

	return 0;
}


