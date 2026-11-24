#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 5000
#define MAXLINE 1000



#define BUFFER_LEN 100


// make sure that every message sent ends with '\0'
int main() {
	
    // Create a UDP Socket
    int listenfd = socket(AF_INET, SOCK_DGRAM, 0);  
	struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));	
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
    servaddr.sin_family = AF_INET; 
 
    // bind server address to socket descriptor
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
     
    // receive the message
	struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
	char buffer_read[BUFFER_LEN];
    int n = recvfrom(listenfd, buffer_read, BUFFER_LEN, 0, (struct sockaddr*)&cliaddr, &len);
	
    printf("[SERVER]%s\n", buffer_read);
	
	if(n == BUFFER_LEN) {
		printf("[SERVER] ERROR: message too long\n");
		return 1;
	}
    
    // send the response
	char buffer_write[BUFFER_LEN];	
	strcpy(buffer_write, "Hello from server");
    sendto(listenfd, buffer_write, strlen(buffer_write) + 1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
	
	strcpy(buffer_write, "DISCONNECT");
	sendto(listenfd, buffer_write, strlen(buffer_write) + 1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
	
	return 0;
}
