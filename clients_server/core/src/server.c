#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "config.h"
#include "cJSON.h"
#include "socket.h"
#include "server.h"

#define ADM_NONE 0
#define ADM_QUIT 1

#define SIG_NONE 0
#define SIG_QUIT 1

#define MESS_NONE 0

volatile sig_atomic_t sig_received = SIG_NONE;


int serv_mess_hdl(int fd, char *message) {
	printf("[serv_mess_hdl] [fd:%d][len:%ld][m:%s]\n", fd, strlen(message), message);

	if(strcmp(message, "MESSOK") == 0) {
		;
	}
	else {
		write(fd, "MESSOK", sizeof(char) * 7);
	}

	return 0;
}

int serv_adm_hdl(int fd, char *message) {
	if(!strcmp(message, "QUIT")) {
		fprintf(stdout, "[serv_adm_hdl] QUIT received.\n\nExiting ...\n\n");
	}

	fprintf(stderr, "[serv_adm_hdl][fd:%d][len:%ld][m:%s]\n", fd, strlen(message), message);

	return 0;
}

void serv_sig_catch(int signal) {
	if(signal == SIGINT) {
		fprintf(stdout, "\n[serv_sig_catch] SIGINT received.\n\nExiting ...\n\n");
		sig_received = SIG_QUIT;
	}
}

/* 
 * Read socket until the end and return has single string.
 * Can result of multiple messages or incomplete messages.
 * Provide incomplete message at next call.
 * */
char *serv_read_socket(int fd, char *incomplete) {
	int n_read;
	int read_size = 128;
	int alloc_len;
	int len;
	char *res;

	/* from last socket read */
	if(incomplete) {
		alloc_len = strlen(incomplete) + read_size;
		len = strlen(incomplete);
		res = malloc(sizeof(char) * (alloc_len + 1));
		strcpy(res, incomplete);
	}
	else {
		alloc_len = read_size;
		len = 0;
		res = malloc(sizeof(char) * (alloc_len + 1));
	}

	n_read = read(fd, res, read_size);

	while(n_read > 0) {
		len += n_read;

		/* realloc */
		while(len + read_size >= alloc_len) {
			alloc_len *= 2;
			res = realloc(res, sizeof(char) * (alloc_len + 1));
			if(!res) {
				fprintf(stdout, "serv_read_socket : call to realloc returned NULL\n");
				return NULL;
			}
		}

		n_read = read(fd, res + len, read_size);
	}

	if(n_read < 0) {
		fprintf(stdout, "serv_read_socket : call to read returned %d\n", n_read);
		return NULL;
	}

	res[len] = '\0';

	return res;
}


int server_launch(char *confpath, int (*mess_hdl)(int, char *), int (*adm_hdl)(int, char *), void (*sig_catch)(int)) {

	cJSON *conf;

	/* admin messages */
	int adm_fd;
	char *adm_cur_read = NULL;
	char *adm_incomplete = NULL;
	char *adm_complete = NULL;

	/* clients connections, fds[0] is server socket */
	static struct pollfd fds[SERV_MAX_CLI + 1];
	char *cli_cur_read = NULL;
	char *cli_complete = NULL;
	static char *cli_incomplete[SERV_MAX_CLI + 1];

	signal(SIGINT, sig_catch);

	conf = config_new(confpath);
	if(!conf) {
		return 1;
	}
	printf("%s\n", cJSON_Print(conf));

	cJSON *c_adm_fd = cJSON_GetObjectItemCaseSensitive(conf, "adm_rd");

	adm_fd = open(c_adm_fd->valuestring, O_RDONLY | O_NONBLOCK);
	if(adm_fd <= 0) {
		perror("server_launch : open error");
		fprintf(stderr, "server_launch : %d\n", adm_fd);
		return 1;
	}

	if(!sock_init_poll(fds, 12345, SERV_MAX_CLI)) {
		fprintf(stderr, "server_launch : call to sock_init_poll returned NULL\n");
		return 1;
	}

	while(1) {
		if(sig_received == SIG_QUIT) {
			break;
		}

		/* Read admin fifo */
		adm_cur_read = serv_read_socket(adm_fd, adm_incomplete);
		fprintf(stdout, "server_launch : adm_cur_read = %s\n", adm_cur_read);
		// TODO: 
		// - decide whether the message is complete or not
		// - extract incomplete from current_read 
		// - provide incomplete for next call
		adm_hdl(adm_fd, adm_complete);


		// Use poll to wait for activity on sockets
		if(poll(fds, SERV_MAX_CLI + 1, -1) < 0) {
			perror("server_launch : poll failed");
			return 1;
		}

		// if server socket has activity, try to accept new connections
		if(fds->revents & POLLIN) {
			sock_accept(&fds[0], SERV_MAX_CLI);
		}

		/* Read sockets and response to incoming messages */
		for(int i = 1; i < SERV_MAX_CLI + 1; i++) {
			if(fds[i].fd > 0 && fds[i].revents & POLLIN) {

				cli_cur_read = serv_read_socket(fds[i].fd, cli_incomplete[i]);
		fprintf(stdout, "server_launch : cli_cur_read = %s\n", cli_cur_read);

				// TODO: 
				// - decide whether the message is complete or not
				// - extract complete+incomplete from current_read 
				// - provide incomplete for next call
				mess_hdl(fds[i].fd, cli_complete);
			}
		}
	}

	close(adm_fd);
	cJSON_Delete(conf);
	sock_close_poll(fds, SERV_MAX_CLI);

	return 0;
}


