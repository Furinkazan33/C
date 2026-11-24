#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "server.h"






void response_set(response *r, char *type, char *code, char *content, size_t len) {
	strcpy(r->version, "HTTP/1.1");
	strcpy(r->code, code);
	r->length = len;
	strcpy(r->type, type);
	strcat(r->type, "; charset=utf-8");
	r->content = content;
}

void response_to_str(char *dest, response r) {
	char itoa[8];
	sprintf(itoa, "%d", r.length);
	
	strcpy(dest, r.version); strcat(dest, " ");
	strcat(dest, r.code); strcat(dest, "\n");
	strcat(dest, "Content-Length: "); strcat(dest, itoa); strcat(dest, "\n");
	strcat(dest, "Content-Type: "); strcat(dest, r.type); strcat(dest, "\n\n");	
	strcat(dest, r.content);
}

req_header *serv_get_req_header(req_header *dest, char *req) {
	char *p = req;
	char *d = dest->path;
	
	if(!strncmp(req, "GET ", 4)) {
		p+=4;
		strcpy(dest->type, "GET");
	}
	else if(!strncmp(req, "POST ", 5)) {
		p+=5;
		strcpy(dest->type, "POST");
	}
	
	for(*d = 0; *p && *p != ' ' && *p != '\n'; p++, d++) {
		*d = *p;
	}
	*d = 0;
	
	if(*p == ' ') {
		return dest;
	}
	else {
		LOG_E("malformed request:[%s]\n", dest->path)
		return NULL;
	}
}




/*
struct sockaddr {
	sa_family_t     sa_family;      // Address family
	char            sa_data[];      // Socket address
};

struct sockaddr_in {
	sa_family_t     sin_family;     // AF_INET
	in_port_t       sin_port;       // Port number
	struct in_addr  sin_addr;       // IPv4 address
};

struct pollfd {
	int   fd;         // file descriptor
	short events;     // requested events
	short revents;    // returned events
};


*/
int server(uint16_t sock_port, uint8_t sock_queue_len, int (*block)(int)) {
	#ifdef DEBUG
		LOG_S("starting...\n");
	#endif
	
	server_params params = { true, false, false };
	
	// admin fifo
	char *fifo = "/tmp/serv_fifo";
	if (access(fifo, F_OK) != 0) {
		IF_ERR(mkfifo(fifo, S_IRWXU | S_IRWXG) < 0, return 1;)
		#ifdef DEBUG
			LOG_S("fifo [%s] created\n", fifo);
		#endif
	}
	int fd_fifo = open(fifo, O_NONBLOCK); IF_ERR(fd_fifo < 0, return 1;)
	
	#ifdef DEBUG
		LOG_S("reading fifo [%s] on [%d]\n", fifo, fd_fifo);
	#endif
	
	struct sockaddr_in address;
	address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(sock_port);

	#define MAX_EVENTS 10
	struct epoll_event ev, events[MAX_EVENTS];
	int listen_sock, conn_sock, nfds, epollfd;
	
	
	// socket creation
	listen_sock = socket(AF_INET, SOCK_STREAM, 0); IF_ERR(listen_sock < 0, return 1;)
	#ifdef DEBUG
		LOG_S("socket created on [%d]\n", listen_sock);
	#endif
	
	//bind
	int ret_bind = bind(listen_sock, (struct sockaddr *) &address, sizeof(address)); IF_ERR(ret_bind < 0, return 1;)
	#ifdef DEBUG
		LOG_S("binding on [%d]\n", listen_sock);
	#endif
	
	//listen
	int ret_listen = listen(listen_sock, sock_queue_len); IF_ERR(ret_listen < 0, return 1;)
	#ifdef DEBUG
		LOG_S("listening on [%d]\n", listen_sock);
	#endif
	
	
	epollfd = epoll_create1(0); IF_ERR(epollfd < 0, return 1;)
	ev.events = EPOLLIN;
	ev.data.fd = listen_sock;
	
	IF_ERR(epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1, return 1;)

	
	
	while(!params.stop) {
		
		/* admin fifo */
		{
			char tmp[SERV_FIFO_MESS_LEN + 1];
			int n_read = read(fd_fifo, tmp, SERV_FIFO_MESS_LEN + 1); IF_ERR(n_read < 0, return 1;)
			
			if(n_read > 0) {
				#ifdef DEBUG
					LOG_S("read fifo:[%s]\n", tmp);
				#endif
				
				if(!strncmp(tmp, "stop", 4)) {
					params.stop = true;
				}
				else if(!strncmp(tmp, "accept", 6)) {
					params.accept = true;
				}
				else if(!strncmp(tmp, "refuse", 6)) {
					params.accept = false;
				}
			}
		}
		
		if(params.accept) {
		   nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1); IF_ERR(nfds < 0, return 1;)

		   for (int n = 0; n < nfds; ++n) {
			   if (events[n].data.fd == listen_sock) {
				   socklen_t addrlen;
				   conn_sock = accept(listen_sock, (struct sockaddr *) &address, &addrlen);
				   #ifdef DEBUG
						LOG_S("accepting on [%d] as [%d]\n", listen_sock, conn_sock);
					#endif
					if (conn_sock == -1) {
						perror("accept");
						exit(EXIT_FAILURE);
					}
				   
					// set socket non-blocking
					int status = fcntl(conn_sock, F_SETFL, fcntl(conn_sock, F_GETFL, 0) | O_NONBLOCK);
					IF_ERR(status < 0, return 1;)

					ev.events = EPOLLIN | EPOLLET;
					ev.data.fd = conn_sock;
					IF_ERR(epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) < 0, return 1;)
					
			   } else {
					int block_ret = block(conn_sock); IF_LOG_E_MESS(block_ret < 0, close(conn_sock); return 1;, "error from block");
					
					close(conn_sock);
			   }
		   }
		}
	}
	
	close(listen_sock);
	close(fd_fifo);
	
	return 0;
}