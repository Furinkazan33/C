#include <stdio.h>
#include <fcntl.h>
#include <signal.h>

#include "include/fifo.h"
#include "include/config.h"

static int sigint = 0;
static int admin_quit = 0;

static int admin_message(char *message);
static void sig_catch(int signal);


int main(void) {
	int fd_rd;
	config *conf;

	if(signal(SIGINT, sig_catch) == SIG_ERR) {
		perror("main : error setting signal handler");
		return 1;
	}

	conf = config_read("config/init.cfg", "config");
	if(!conf) {
		return 1;
	}
	//conf_print(conf);

	fd_rd = open("./dev/fifo_rd", O_RDONLY);
	if(!fd_rd) {
		perror("");
		return 1;
	}

	/* Main loop */
	while(1) {
		if(sigint || admin_quit) {
			break;
		}

		fifo_read(fd_rd, ';', admin_message);

		sleep(1);
	}

	/* free ressources */
	close(fd_rd);
	conf_free(conf);

	return 0;
}


static int admin_message(char *message) {
	if(!strcmp(message, "QUIT")) {
		fprintf(stdout, "[admin_message] QUIT received.\n\nExiting ...\n\n");
		admin_quit = 1;
		return 0;
	}

	fprintf(stderr, "[FIFO_RD]:%s:\n", message);
	return 1;
}

static void sig_catch(int signal) {
	if(signal == SIGINT) {
		fprintf(stdout, "\n[sig_catch] SIGINT received.\n\nExiting ...\n\n");
		sigint = 1;
	}
}

