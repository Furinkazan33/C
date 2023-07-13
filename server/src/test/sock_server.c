#include "../include/sock_server.h"


int mess_handler(char *message, int fd) {
	printf("mess_handler : client %d : %s\n", fd, message);

	if(strcmp(message, "MESSOK") == 0) {
		;
	}
	else {
		write(fd, "MESSOK;", sizeof(char) * 8);
	}

	return 1;
}


int main() {
	/* +1 for server socket */
	struct pollfd fds[MAX_CLIENTS + 1];
	char buffer[MAX_CLIENTS + 1][MESS_BUF_LEN];
	struct pollfd *free_fds;

	memset(buffer, 0, MESS_BUF_LEN * (MAX_CLIENTS + 1));

	if(!sock_serv_init(fds)) {
		return 1;
	}

	while (1) {
		// Use poll to wait for activity on sockets
		if (poll(fds, MAX_CLIENTS + 1, -1) < 0) {
			perror("Poll failed");
			return 0;
		}

		// if server socket has activity, try to accept new connections
		if (fds->revents & POLLIN) {
			/* next avalaible fds */
			free_fds = NULL;
			for(int i = 1; i < MAX_CLIENTS + 1; i++) {
				if(fds[i].fd == 0) {
					free_fds = &fds[i];
				}
			}
			sock_serv_accept(fds[0].fd, free_fds);
		}

		/* Read sockets and response to incoming messages */
		if(!sock_serv_rw(fds, &buffer[0][0], mess_handler)) {
			return 0;
		}
	}

	sock_serv_close(fds);

	return 0;
}


