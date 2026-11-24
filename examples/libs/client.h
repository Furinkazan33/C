#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include "macros.h"


#ifndef CLI_MESS_LEN
	#define CLI_MESS_LEN 2048
#endif

#ifndef SERV_MESS_LEN
	#define SERV_MESS_LEN 65536
#endif


#define GET(fd, message, url, host, user_agent, accept) { \
	strcpy(message, "GET "); strcat(message, url); strcat(message, " HTTP/1.1\n"); \
	strcat(message, "Host: "); strcat(message, host); strcat(message, "\n"); \
	strcat(message, "User-Agent: "); strcat(message, user_agent); strcat(message, "\n"); \
	strcat(message, "Accept: "); strcat(message, accept); strcat(message, "\n"); \
	\
	SEND(fd, message) \
	\
}

#define POST(fd, message, url, host, type, content_length, data) { \
	strcpy(message, "POST "); strcat(message, url); strcat(message, " HTTP/1.1\n"); \
	strcat(message, "Host: "); strcat(message, host); strcat(message, "\n"); \
	strcat(message, "Content-Type: "); strcat(message, type); strcat(message, "\n"); \
	strcat(message, "Content-Length: "); strcat(message, content_length); strcat(message, "\n"); \
	strcat(message, "\n"); \
	strcat(message, "data"); \
	\
	SEND(fd, message) \
	\
}

#ifdef DEBUG_CLI
	#define SEND(fd, mess) { \
		LOG(stdout, "request[%s]\n", (mess)); \
		int ret_send = send(fd, mess, strlen(mess), 0); \
		IF_ERR(ret_send < 0, return 1;); \
	}

	#define READ(fd, mess) { \
		int len = read(fd, mess, SERV_MESS_LEN); \
		IF_ERR(len < 0, return 1;); \
		if(len > 0) { \
			LOG(stdout, "response[%s]\n", (mess)); \
		} \
	}
#else
	#define SEND(fd, mess) { \
		int ret_send = send(fd, mess, strlen(mess), 0); \
		IF_ERR(ret_send < 0, return 1;); \
	}
	
	#define READ(fd, mess) { \
		int len = read(fd, mess, SERV_MESS_LEN); \
		IF_ERR(len < 0, return 1;); \
	}
#endif


int client(char *host, uint16_t sock_port, int (*block)(int, char *));

