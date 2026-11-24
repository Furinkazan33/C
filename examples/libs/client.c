#include "client.h"




int client(char *host, uint16_t sock_port, int (*block)(int, char *)) {
	// socket creation
    int fd = socket(AF_INET, SOCK_STREAM, 0); IF_ERR(fd < 0, return 1;);
	
	struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(sock_port);

    // Convert IPv4 and IPv6 addresses from text to binary
    int ret_inet = inet_pton(AF_INET, host, &serv_addr.sin_addr); IF_ERR(ret_inet < 0, return 1;);
	#ifdef DEBUG_CLI
		LOG_S("inet_pton ok\n");
	#endif
	
	int status = connect(fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)); IF_ERR(status < 0, return 1;);
	
	#ifdef DEBUG_CLI
		LOG_S("connected to server\n");
	#endif
	
	int ret_block = block(fd, host); IF_LOG_E_MESS(ret_block < 0, close(fd); return 1, "error from block");


    close(fd);
	
	return 0;
}
