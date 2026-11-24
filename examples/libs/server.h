#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <assert.h>
#include "macros.h"

// admin
#ifndef SERV_FIFO_MESS_LEN
	#define SERV_FIFO_MESS_LEN 128
#endif

// clients requests
#ifndef SERV_REQ_MESS_LEN
	#define SERV_REQ_MESS_LEN 2048
#endif

#ifndef SERV_REQ_MAX_TYPE_LEN
	#define SERV_REQ_MAX_TYPE_LEN 16
#endif

#ifndef SERV_REQ_MAX_PATH_LEN
	#define SERV_REQ_MAX_PATH_LEN 1024
#endif

#ifndef SERV_MAX_CLIENTS
	#define SERV_MAX_CLIENTS 2
#endif


// server responses
//TODO: change to dynamic
#ifndef SERV_RESP_MESS_LEN
	#define SERV_RESP_MESS_LEN 65536
#endif

#ifndef SERV_RESP_MAX_CODE_LEN
	#define SERV_RESP_MAX_CODE_LEN 32
#endif



#define xstr(s) str(s)
#define str(...) #__VA_ARGS__
#define tag(name, ...) <name>__VA_ARGS__</name>

#ifdef DEBUG
	#define SEND(fd, type, code, content) { \
		response response; \
		response_set(&response, type, code, content, strlen(content)); \
		char rcontent[SERV_RESP_MESS_LEN]; \
		response_to_str(rcontent, response); \
		LOG(stdout, "response[%s]\n", (rcontent)); \
		int ret_send = send(fd, rcontent, strlen(rcontent), 0); \
		IF_ERR(ret_send < 0, return 1;); \
	}

	#define READ(fd, request) { \
		memset(request, 0, SERV_REQ_MESS_LEN); \
		int len = read(fd, request, SERV_REQ_MESS_LEN); \
		IF_ERR(len < 0, return 1;); \
		if(request[SERV_REQ_MESS_LEN - 1] != 0) { \
			LOG_E("request too long\n")\
			return -1; \
		} \
		if(len > 0) { \
			LOG(stdout, "request[%s]\n", (request)); \
		} \
	}
#else
	#define SEND(fd, type, code, content) { \
		response response; \
		response_set(&response, type, code, content, strlen(content)); \
		char rcontent[SERV_RESP_MESS_LEN]; \
		response_to_str(rcontent, response); \
		int ret_send = send(fd, rcontent, strlen(rcontent), 0); \
		IF_ERR(ret_send < 0, return 1;); \
	}

	#define READ(fd, request) { \
		memset(request, 0, SERV_REQ_MESS_LEN); \
		int len = read(fd, request, SERV_REQ_MESS_LEN); \
		IF_ERR(len < 0, return 1;); \
		if(request[SERV_REQ_MESS_LEN - 1] != 0) { \
			LOG_E_MESS("request too long\n")\
			return -1; \
		} \
	}
#endif



typedef struct response {
	char type[128];
	char code[SERV_RESP_MAX_CODE_LEN];
	int length;
	char version[20];
	char *content;
}  response;


typedef struct req_header {
	char type[SERV_REQ_MAX_TYPE_LEN];
	char path[SERV_REQ_MAX_PATH_LEN];
} req_header;

void response_set(response *r, char *type, char *code, char *content, size_t len);
void response_to_str(char *dest, response r);
req_header *serv_get_req_header(req_header *dest, char *req);


typedef struct server_params {
	bool accept;
	bool stop;
	bool restart;
} server_params;

/*
to use port 80:
sudo setcap CAP_NET_BIND_SERVICE=+eip ./build/test_server
*/

int server(uint16_t sock_port, uint8_t sock_queue_len, int (*block)(int fd));

