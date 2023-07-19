#include <signal.h>
#include "../include/sock_server.h"

#define SERV_BUF_SIZE 1024
#define SERV_MESS_SIZE 10

int serv_mess_handler(char *message, int fd) {
	printf("server received [%s] from client on [%d]\n", message, fd);

	if(strcmp(message, "MESSOK") == 0) {
		;
	}
	else {
		write(fd, "MESSOK", sizeof(char) * 7);
	}

	return 1;
}

static int sigint = 0;
static void sig_catch(int signal);

int main() {
	/* clients connections */
	/* +1 for server socket */
	struct pollfd fds[MAX_CLIENTS + 1];
	char messages[MAX_CLIENTS + 1][SERV_MESS_SIZE];
	char buffers[MAX_CLIENTS + 1][SERV_BUF_SIZE];
	size_t m_pos[MAX_CLIENTS + 1] = { 0 };
	size_t b_pos[MAX_CLIENTS + 1] = { 0 };
	int ret;

	signal(SIGINT, sig_catch);

	if(!sock_serv_init(fds, 12345)) {
		return 1;
	}

	while (1) {
		if(sigint) {
			break;
		}

		// Use poll to wait for activity on sockets
		if (poll(fds, MAX_CLIENTS + 1, -1) < 0) {
			perror("Poll failed");
			return 0;
		}

		// if server socket has activity, try to accept new connections
		if (fds->revents & POLLIN) {
			sock_serv_accept(&fds[0]);
		}

		/* Read sockets and response to incoming messages */
		for (int i = 1; i < MAX_CLIENTS + 1; i++) {
			if(fds[i].fd > 0 && fds[i].revents & POLLIN) {

				ret = frd_read(fds[i].fd, serv_mess_handler, messages[i], SERV_MESS_SIZE, &m_pos[i], buffers[i], SERV_BUF_SIZE, &b_pos[i]);

				switch(ret) {
					case FRD_READ_ERROR:
					perror("serv_main : read error");
					return 1;
					break;

					case FRD_EOF:
					fprintf(stdout, "serv_main : client disconnected\n");
					close(fds[i].fd);
					fds[i].fd = 0;
					fds[i].events = 0;
					break;

					case FRD_TOO_LONG:
					fprintf(stderr, "serv_main : message too long : [%s], [%s]\n", messages[i], &buffers[i][b_pos[i]]);
					close(fds[i].fd);
					fds[i].fd = 0;
					fds[i].events = 0;
					break;

					default:
					break;
				}
			}
		}
	}

	sock_serv_close(fds);

	return 0;
}

static void sig_catch(int signal) {
	if(signal == SIGINT) {
		fprintf(stdout, "\n[sig_catch] SIGINT received.\n\nExiting ...\n\n");
		sigint = 1;
	}
}

