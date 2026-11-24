#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "macros.h"



int block(int fd) {
	req_header header;
	char request[SERV_REQ_MESS_LEN];

	READ(fd, request)
	
	
	if(strlen(request) > 0) {
		IF_LOG_E_MESS(serv_get_req_header(&header, request) == NULL, return 1;, "error from serv_get_req_header")
	
		LOG_S("req:type=[%s], path=[%s]\n", header.type, header.path)
		
		if(!strcmp(header.type, "GET")) {
			if(!strcmp(header.path, "/json")) {
				SEND(fd, "application/json", "200 OK", "{ name: \"toto\"}")
			}
			else if(!strcmp(header.path, "/")) {
				SEND(fd, "text/html", "200 OK", xstr(
				tag(
					html, 
						tag(body, 
							tag(p1, Hello, world !) 
							tag(br, ) 
							tag(p1, Hello, world !)
						)
					)
				))
			}
			else {
				SEND(fd, "text/plain", "404 Not Found", "")
			}
		}
	}
	
	
	return 0;
}



int main(int argc, char **argv) {
	printf("%ld %ld %ld\n", sizeof(response), sizeof(char *), sizeof(int));
	
	
	
	if(argc != 2) {
		LOG_E("usage: %s port\n", argv[0]);
		return 1;
	}

	int ret = server(atoi(argv[1]), 4, block); IF_LOG_E_MESS(ret < 0, return 1;, "error from server")

	
	return EXIT_SUCCESS;
}
