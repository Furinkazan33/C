#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

int connect_to_server(char *adress, int port, int client_fd) {
	int ret;
	struct sockaddr_in sa;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(adress);
	sa.sin_port = htons(port);

	ret = connect(client_fd, (struct sockaddr *)&sa, sizeof(sa));

	if(ret < 0) {
		perror("connect_to_server : connection failed");
	}
	else {
		printf("Connected to server [%s]:[%d]\n", adress, port);
	}

	return ret;
}

int new_socket(int *client_fd) {
	*client_fd = socket(AF_INET, SOCK_STREAM, 0);

	if(*client_fd < 0) {
		perror("new_socket : socket creation failed");
	}
	return *client_fd;
}

int main() {
	char buffer[1024];
	int bytesRead;
	int client_fd;

	puts("");

	if (new_socket(&client_fd) < 0) {
		exit(EXIT_FAILURE);
	}

	if(connect_to_server("127.0.0.1", 12345, client_fd) < 0) {
		exit(EXIT_FAILURE);
	}

	// Set up file descriptor sets for select
	fd_set readSet;
	FD_ZERO(&readSet);
	FD_SET(client_fd, &readSet);

	// Use select to wait for activity on sockets
	if (select(client_fd + 1, &readSet, NULL, NULL, NULL) < 0) {
		perror("main : select failed");
		exit(EXIT_FAILURE);
	}

	// Check if the client socket has activity
	while (FD_ISSET(client_fd, &readSet)) {

		memset(buffer, 0, sizeof(buffer));
		bytesRead = read(client_fd, buffer, sizeof(buffer));

		if (bytesRead > 0) {

			printf("Received from server: %s\n", buffer);
			// Process server response
			// ...
		}
		else if (bytesRead == 0) {
			printf("Server disconnected\n");
			close(client_fd);
			client_fd = -1;
		}
		else {
			perror("Read failed");
			close(client_fd);
			client_fd = -1;
		}
	}
	return 0;
}


