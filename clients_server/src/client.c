#include "../include/sock_client.h"

#define CLI_BUF_SIZE 1024
#define CLI_MESS_SIZE 10

int mess_treat(char *message, int fd) {
	printf("client received [%s] from server on [%d]\n", message, fd);

	if(strcmp(message, "MESSOK") == 0 || strcmp(message, "CONOK") == 0) {

	}
	else {
		write(fd, "MESSOK", 7);
	}
	return 1;
}

int main(int argc, char **argv) {
	int fd, ret, eot;
	struct sockaddr_in sa;
	fd_set read_set;
	char message[CLI_MESS_SIZE];
	char buffer[CLI_BUF_SIZE];
	size_t m_pos = 0;
	size_t b_pos = 0;


	if(argc < 2) {
		fprintf(stderr, "usage : %s client_id\n", argv[0]);
		return 1;
	}

	if(!sock_net_new(&fd)) {
		return 1;
	}

	if(!sock_connect_to_serv("127.0.0.1", 12345, fd, &sa)) {
		return 1;
	}

	if(!sock_select(fd, &read_set)) {
		return 1;
	}

	eot = 1;
	while(eot) {
		if(FD_ISSET(fd, &read_set)) {
			ret = frd_read(fd, mess_treat, &message[0], CLI_MESS_SIZE, &m_pos, &buffer[0], CLI_BUF_SIZE, &b_pos);

			switch(ret) {
				case FRD_NOT_FINISHED:
				fprintf(stdout, "cli_main : message not finished. Waiting for the rest.\n");
				break;

				case FRD_EOB:
				fprintf(stdout, "cli_main : end of buffer. All messages treated.\n");
				break;

				case FRD_READ_ERROR:
				perror("cli_main : read error. END.");
				eot = 0;
				break;

				case FRD_EOF:
				fprintf(stdout, "cli_main : server disconnected. END.\n");
				eot = 0;
				break;

				case FRD_TOO_LONG:
				fprintf(stderr, "cli_main : message too long from server. Reporting this. END.\n");
				/* TODO: impossible ? */
				eot = 0;
				break;

				default:
				fprintf(stderr, "cli_main : unknown return code : %d. END.\n", ret);
				eot = 0;
				break;
			}
		}
		write(fd, "test AAA", 9);
		write(fd, "test BBB too long !", 20);
	}

	close(fd);

	return 0;
}
