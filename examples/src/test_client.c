#include <stdio.h>
#include <string.h>
#include "client.h"
#include "macros.h"



int block(int fd, char *host) {
	char message[CLI_MESS_LEN];
	char message_server[SERV_MESS_LEN] = { 0 };
	int len;
	
	GET(fd, message, "/", host, "home made", "*/*")
	len = 0;
	while(len == 0) {
		len = read(fd, message_server, SERV_MESS_LEN); IF_ERR(len < 0, return 1;);
	}
	#ifdef DEBUG_CLI
		LOG(stdout, "response[%s]\n", (message_server));
	#endif
	
	/*
	status = connect(fd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr)); IF_ERR(status < 0, return 1;);
	
	GET(fd, message, "/json", host, "home made", "\*\/\*")
	len = 0;
	while(len == 0) {
		len = read(fd, message_server, SERV_MESS_LEN); IF_ERR(len < 0, return 1;);
	}
	#ifdef DEBUG_CLI
		LOG(stdout, "response[%s]\n", (message_server));
	#endif
	
	
	status = connect(fd, (struct sockaddr *) serv_addr, sizeof(struct sockaddr)); IF_ERR(status < 0, return 1;);
	
	GET(fd, message, "/blabla", host, "home made", "\*\/\*")
	len = 0;
	while(len == 0) {
		len = read(fd, message_server, SERV_MESS_LEN); IF_ERR(len < 0, return 1;);
	}
	#ifdef DEBUG_CLI
		LOG(stdout, "response[%s]\n", (message_server));
	#endif
	*/
	
	
	return 0;
}


int main(int argc, char **argv) {
	if(argc != 3) {
		LOG_E("usage: %s adress port\n", argv[0]);
		return 1;
	}

	IF_LOG_E_MESS(client(strcmp(argv[1], "localhost") ? "127.0.0.1" : argv[1], atoi(argv[2]), block) < 0, 
		return 1;, "error from client")


    return 0;
}
