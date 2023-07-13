#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define MESS_BUF_LEN 10

int sock_cli_connect(char *adress, int port, int fd, struct sockaddr_in *sa) {
	memset(sa, 0, sizeof(*sa));
	sa->sin_family = AF_INET;
	sa->sin_addr.s_addr = inet_addr(adress);
	sa->sin_port = htons(port);

	if(connect(fd, (struct sockaddr *)sa, sizeof(*sa)) < 0) {
		perror("sock_connect_to_server : connection failed");
		return 0;
	}

	printf("Connected to server [%s]:[%d]\n", adress, port);
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

int sock_cli_rw(int fd, fd_set *read_set, char *buffer, int (*mess_handler)(char *, int)) {
	size_t bytesRead;
	char tmp[MESS_BUF_LEN];
	char *eom;
	int len;

	if(FD_ISSET(fd, read_set)) {
		memset(tmp, 0, MESS_BUF_LEN);
		bytesRead = read(fd, tmp, MESS_BUF_LEN);

		if (bytesRead > 0) {
			eom = strchr(tmp, '\0');

			/* delim found => buffer is now a complete message */
			if(eom) {
				if(strlen(buffer) + eom - tmp >= MESS_BUF_LEN - 1) {
					fprintf(stderr, "Message too long from server :%s:%s:\n", buffer, tmp);
					memset(buffer, 0, MESS_BUF_LEN);
					return 0;
				}

				strncat(buffer, tmp, eom - tmp);
				mess_handler(buffer, fd);
				memset(buffer, 0, MESS_BUF_LEN);

				eom++;
				if(eom - tmp < bytesRead) {
					strcpy(buffer, eom);
				}
			}
			else {
				if(strlen(buffer) + strlen(tmp) >= MESS_BUF_LEN - 1) {
					fprintf(stderr, "Message too long from server :%s:%s:\n", buffer, tmp);
					memset(buffer, 0, MESS_BUF_LEN);
					return 0;
				}
				strncat(buffer, tmp, strlen(tmp));
			}
		}
		else if (bytesRead == 0) {
			printf("Server disconnected\n");
			memset(buffer, 0, MESS_BUF_LEN);
			return 0;
		}
		else {
			perror("sock_read_write : read failed");
			memset(buffer, 0, MESS_BUF_LEN);
			return 0;
		}
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


