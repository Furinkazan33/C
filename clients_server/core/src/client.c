#include <string.h>
#include <unistd.h>
#include "client.h"
#include "socket.h"
#include "fr.h"


void mess_treat(int fd, char *message, int len, int *rc) {
	*rc = 0;

	printf("client received [%s] of len [%d] from server on [%d]\n", message, len, fd);

	if(strcmp(message, "MESSOK") == 0) {

	}
	else if (strcmp(message, "CONOK") == 0) {
		puts("ici");
		char tmp[20];
		sprintf(tmp, "Hello from client");
		write(fd, tmp, strlen(tmp));
	}
	else {
	}
}

int launch_client(int client_id) {
	int fd, ret, eot, mess_rc;
	struct sockaddr_in sa;
	fd_set read_set;
	char message[CLI_MESS_SIZE];
	char buffer[CLI_BUF_SIZE];
	size_t m_pos = 0;
	size_t b_pos = 0;

	if(!sock_net_new(&fd)) {
		return 1;
	}

	if(!sock_connect_to_serv("127.0.0.1", 12345, fd, &sa)) {
		return 1;
	}

	sprintf(message, "Hello from client %d", client_id);
	write(fd, message, strlen(message));


	if(!sock_select(fd, &read_set)) {
		return 1;
	}

	eot = 1;
	while(eot) {
		if(FD_ISSET(fd, &read_set)) {
			ret = fr_part_read(fd, mess_treat, &message[0], CLI_MESS_SIZE, &m_pos, &buffer[0], CLI_BUF_SIZE, &b_pos, &mess_rc);

			switch(ret) {
				case FR_RC_NOT_FINISHED:
					fprintf(stdout, "cli_main : message not finished. Waiting for the rest.\n");
					break;

				case FR_RC_EOB:
					fprintf(stdout, "cli_main : end of buffer. All messages treated.\n");
					break;

				case FR_RC_READ_ERROR:
					perror("cli_main : read error. END.");
					eot = 0;
					break;

				case FR_RC_EOF:
					fprintf(stdout, "cli_main : server disconnected. END.\n");
					eot = 0;
					break;

				case FR_RC_TOO_LONG:
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
	}

	close(fd);

	return 0;
}


