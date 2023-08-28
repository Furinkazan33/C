#include <fcntl.h>
#include <signal.h>
#include "config.h"
#include "cJSON.h"
#include "socket.h"
#include "fr.h"
#include "usage.h"

#define SERV_BUF_SIZE 1024
#define SERV_MESS_SIZE 10
#define SERV_MAX_CLI 2


static int sigint = 0;
static int adm_quit = 0;

void serv_mess_handler(int fd, char *message, int len);
static void adm_handler(int fd, char *message, int len);
static void sig_catch(int signal);

int main(int argc, char **argv) {
	if(argc != 2) {
		usage(argc, argv, "", "config_file_path");
		exit(1);
	}

	cJSON *conf;

	/* admin messages */
	int adm_fd;
	char adm_m[SERV_MESS_SIZE];
	char adm_b[SERV_BUF_SIZE];
	size_t adm_m_pos = 0;
	size_t adm_b_pos = 0;

	/* clients connections */
	/* +1 for server socket */
	struct pollfd fds[SERV_MAX_CLI + 1];
	char messages[SERV_MAX_CLI + 1][SERV_MESS_SIZE];
	char buffers[SERV_MAX_CLI + 1][SERV_BUF_SIZE];
	size_t m_pos[SERV_MAX_CLI + 1] = { 0 };
	size_t b_pos[SERV_MAX_CLI + 1] = { 0 };
	int ret;

	signal(SIGINT, sig_catch);

	conf = config_new(argv[1]);
	if(!conf) {
		return 1;
	}
	printf("%s\n", cJSON_Print(conf));

	cJSON *c_adm_fd = cJSON_GetObjectItemCaseSensitive(conf, "adm_rd");

	adm_fd = open(c_adm_fd->valuestring, O_RDONLY | O_NONBLOCK);
	if(adm_fd <= 0) {
		perror("");
		return 1;
	}

	if(!sock_init_poll(fds, 12345, SERV_MAX_CLI)) {
		return 1;
	}

	while (1) {
		if(sigint || adm_quit) {
			break;
		}

		/* handle fifo messages */
		int fr_rc = fr_part_read(adm_fd, adm_handler, &adm_m[0], SERV_MESS_SIZE, &adm_m_pos, &adm_b[0], SERV_BUF_SIZE, &adm_b_pos);

		switch(fr_rc) {
			default:
			break;
		}

		// Use poll to wait for activity on sockets
		if (poll(fds, SERV_MAX_CLI + 1, -1) < 0) {
			perror("Poll failed");
			return 0;
		}

		// if server socket has activity, try to accept new connections
		if (fds->revents & POLLIN) {
			sock_accept(&fds[0], SERV_MAX_CLI);
		}

		/* Read sockets and response to incoming messages */
		for (int i = 1; i < SERV_MAX_CLI + 1; i++) {
			if(fds[i].fd > 0 && fds[i].revents & POLLIN) {

				ret = fr_part_read(fds[i].fd, serv_mess_handler, messages[i], SERV_MESS_SIZE, &m_pos[i], buffers[i], SERV_BUF_SIZE, &b_pos[i]);

				switch(ret) {
					case FR_RC_READ_ERROR:
					perror("serv_main : read error");
					return 1;
					break;

					case FR_RC_EOF:
					fprintf(stdout, "serv_main : client disconnected\n");
					close(fds[i].fd);
					fds[i].fd = 0;
					fds[i].events = 0;
					break;

					case FR_RC_TOO_LONG:
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

	close(adm_fd);
	cJSON_Delete(conf);
	sock_close_poll(fds, SERV_MAX_CLI);

	return 0;
}

void serv_mess_handler(int fd, char *message, int len) {
	printf("server read on [%d], length : [%d], content : [%s]\n", fd, len, message);

	if(strcmp(message, "MESSOK") == 0) {
		;
	}
	else {
		write(fd, "MESSOK", sizeof(char) * 7);
	}
}

static void adm_handler(int fd, char *message, int len) {
	if(!strcmp(message, "QUIT")) {
		fprintf(stdout, "[admin_message] QUIT received.\n\nExiting ...\n\n");
	}

	fprintf(stderr, "[admin_message][fd:%d][len:%d][m:%s]\n", fd, len, message);
}
static void sig_catch(int signal) {
	if(signal == SIGINT) {
		fprintf(stdout, "\n[sig_catch] SIGINT received.\n\nExiting ...\n\n");
		sigint = 1;
	}
}
