#include "../include/sock_client.h"

int mess_treat(char *message, int fd) {
	printf("[%d] received [%s] from server\n", fd, message);

	if(strcmp(message, "MESSOK") == 0 || strcmp(message, "CONOK") == 0) {
		
	}
	else {
		write(fd, "MESSOK;", 8);
	}

	return 1;
}

int main(int argc, char **argv) {
	int fd;
	struct sockaddr_in sa;
	fd_set read_set;
	char buffer[MESS_BUF_LEN];
	memset(buffer, 0, MESS_BUF_LEN);

	if(argc < 2) {
		fprintf(stderr, "usage : %s client_id\n", argv[0]);
		return 1;
	}

	if(!sock_cli_init(&fd, "127.0.0.1", 12345, &sa, &read_set)) {
		return 1;
	}

	int test = 0;
	while(1) {
		/* send message to server */
		if(!sock_cli_rw(fd, &read_set, buffer, mess_treat)) {
			break;
		}

		if(test++ == 0) {
			puts("cli test");
			char m[10];
			sprintf(m, "client %s", argv[1]);
			write(fd, m, sizeof(char) * (strlen(m) + 1));
			write(fd, ";", sizeof(char) * 2);
		}
	}

	close(fd);

	return 0;
}
